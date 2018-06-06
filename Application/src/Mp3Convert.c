#include "stm32f10x_lib.h"
#include "ff.h"
#include "mp3dec.h"
#include <string.h>


#define f_tell(fp)		((fp)->fptr)

#define FILE_READ_BUFFER_SIZE 8192
static char file_read_buffer[FILE_READ_BUFFER_SIZE];
volatile int bytes_left;
char *read_ptr;
FIL hMp3;
FIL hWav;

HMP3Decoder				hMP3Decoder;



static uint32_t Mp3ReadId3V2Tag(FIL* pInFile, char* pszArtist, uint32_t unArtistSize, char* pszTitle, uint32_t unTitleSize);


//成功:返回数据的个数
int DecodeOneFrame(int16_t *DecodeBuff)
{
    int offset, err;
    MP3FrameInfo mp3FrameInfo;

    offset = MP3FindSyncWord((unsigned char*)read_ptr, bytes_left);
    bytes_left -= offset;
    read_ptr += offset;

    err = MP3Decode(hMP3Decoder, (unsigned char**)&read_ptr, (int*)&bytes_left, DecodeBuff, 0);
    if (err) {
		/* error occurred */
		switch (err) {
		case ERR_MP3_INDATA_UNDERFLOW:
			return -1;
		case ERR_MP3_MAINDATA_UNDERFLOW:
			/* do nothing - next call to decode will provide more mainData */
			return 0;
		case ERR_MP3_FREE_BITRATE_SYNC:
		default:
			return -1;
		}
	} else {
		// no error
		MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);
		return mp3FrameInfo.outputSamps;
	}
}


int ConvertMp3(char *Mp3FileName, char *WavFileName)
{
    FRESULT res;
    char szArtist[120];
    char szTitle[120];
    unsigned int br, wr;
    static int16_t WriteBuff[1024];
    int DecodeRes;
    int ret = 0;
    
    bytes_left = FILE_READ_BUFFER_SIZE;
    read_ptr = file_read_buffer;

    res = f_open(&hMp3, Mp3FileName, FA_OPEN_EXISTING | FA_READ);
    if(res != FR_OK)
        return -1;

    res = f_open(&hWav, WavFileName, FA_CREATE_NEW | FA_WRITE);
    if(res != FR_OK)
    {
        f_close(&hMp3);
        return -2;
    }

    // Read ID3v2 Tag
    Mp3ReadId3V2Tag(&hMp3, szArtist, sizeof(szArtist), szTitle, sizeof(szTitle));

    // Fill buffer
    f_read(&hMp3, file_read_buffer, FILE_READ_BUFFER_SIZE, &br);

    hMP3Decoder = MP3InitDecoder();

    while(1)
    {
        //判断MP3数据的缓冲中的数据是否已经转换超过了一般
        if(bytes_left < (FILE_READ_BUFFER_SIZE / 2))
        {
            //缓冲内数据已经转换超过了一半，从文件中补充一半的数据
            memcpy(file_read_buffer, file_read_buffer + (FILE_READ_BUFFER_SIZE / 2), (FILE_READ_BUFFER_SIZE / 2));
            read_ptr -= (FILE_READ_BUFFER_SIZE / 2);

            res = f_read(&hMp3, file_read_buffer + (FILE_READ_BUFFER_SIZE / 2), (FILE_READ_BUFFER_SIZE / 2), &br);

            bytes_left += br;

            if(br != (FILE_READ_BUFFER_SIZE / 2) || res != FR_OK)
            {
                //文件读取异常，或者文件读取结束，跳出本流程
                break;
            }
        }

        //对一帧MP3数据进行解码，并写入到Wav文件中
        DecodeRes = DecodeOneFrame(WriteBuff);
        if(DecodeRes > 0)
        {
            res = f_write(&hWav, WriteBuff, DecodeRes * 2, &wr);
            if(res != FR_OK)
            {
                ret = -3;
                goto end;
            }
        }
        else if(DecodeRes < 0)
        {
            ret = -4;
            goto end;
        }
    }

    //将剩余的MP3的数据转换成WAV数据
    while(bytes_left > 0)
    {
        DecodeRes = DecodeOneFrame(WriteBuff);
        if(DecodeRes > 0)
        {
            res = f_write(&hWav, WriteBuff, DecodeRes * 2, &wr);
            if(res != FR_OK)
            {
                ret = -5;
                goto end;
            }
        }
        else if(DecodeRes < 0)
        {
            ret = -6;
            goto end;
        }
    }


end:
    f_close(&hMp3);
    f_close(&hWav);
    return ret;
}

static uint32_t Mp3ReadId3V2Text(FIL* pInFile, uint32_t unDataLen, char* pszBuffer, uint32_t unBufferSize)
{
	UINT unRead = 0;
	BYTE byEncoding = 0;
	if((f_read(pInFile, &byEncoding, 1, &unRead) == FR_OK) && (unRead == 1))
	{
		unDataLen--;
		if(unDataLen <= (unBufferSize - 1))
		{
			if((f_read(pInFile, pszBuffer, unDataLen, &unRead) == FR_OK) ||
					(unRead == unDataLen))
			{
				if(byEncoding == 0)
				{
					// ISO-8859-1 multibyte
					// just add a terminating zero
					pszBuffer[unDataLen] = 0;
				}
				else if(byEncoding == 1)
				{
					// UTF16LE unicode
					uint32_t r = 0;
					uint32_t w = 0;
					if((unDataLen > 2) && (pszBuffer[0] == 0xFF) && (pszBuffer[1] == 0xFE))
					{
						// ignore BOM, assume LE
						r = 2;
					}
					for(; r < unDataLen; r += 2, w += 1)
					{
						// should be acceptable for 7 bit ascii
						pszBuffer[w] = pszBuffer[r];
					}
					pszBuffer[w] = 0;
				}
			}
			else
			{
				return 1;
			}
		}
		else
		{
			// we won't read a partial text
			if(f_lseek(pInFile, f_tell(pInFile) + unDataLen) != FR_OK)
			{
				return 1;
			}
		}
	}
	else
	{
		return 1;
	}
	return 0;
}


static uint32_t Mp3ReadId3V2Tag(FIL* pInFile, char* pszArtist, uint32_t unArtistSize, char* pszTitle, uint32_t unTitleSize)
{
	pszArtist[0] = 0;
	pszTitle[0] = 0;

	BYTE id3hd[10];
	UINT unRead = 0;
	if((f_read(pInFile, id3hd, 10, &unRead) != FR_OK) || (unRead != 10))
	{
		return 1;
	}
	else
	{
		uint32_t unSkip = 0;
		if((unRead == 10) &&
				(id3hd[0] == 'I') &&
				(id3hd[1] == 'D') &&
				(id3hd[2] == '3'))
		{
			unSkip += 10;
			unSkip = ((id3hd[6] & 0x7f) << 21) | ((id3hd[7] & 0x7f) << 14) | ((id3hd[8] & 0x7f) << 7) | (id3hd[9] & 0x7f);

			// try to get some information from the tag
			// skip the extended header, if present
			uint8_t unVersion = id3hd[3];
			if(id3hd[5] & 0x40)
			{
				BYTE exhd[4];
				f_read(pInFile, exhd, 4, &unRead);
				size_t unExHdrSkip = ((exhd[0] & 0x7f) << 21) | ((exhd[1] & 0x7f) << 14) | ((exhd[2] & 0x7f) << 7) | (exhd[3] & 0x7f);
				unExHdrSkip -= 4;
				if(f_lseek(pInFile, f_tell(pInFile) + unExHdrSkip) != FR_OK)
				{
					return 1;
				}
			}
			uint32_t nFramesToRead = 2;
			while(nFramesToRead > 0)
			{
				char frhd[10];
				if((f_read(pInFile, frhd, 10, &unRead) != FR_OK) || (unRead != 10))
				{
					return 1;
				}
				if((frhd[0] == 0) || (strncmp(frhd, "3DI", 3) == 0))
				{
					break;
				}
				char szFrameId[5] = {0, 0, 0, 0, 0};
				memcpy(szFrameId, frhd, 4);
				uint32_t unFrameSize = 0;
				uint32_t i = 0;
				for(; i < 4; i++)
				{
					if(unVersion == 3)
					{
						// ID3v2.3
						unFrameSize <<= 8;
						unFrameSize += frhd[i + 4];
					}
					if(unVersion == 4)
					{
						// ID3v2.4
						unFrameSize <<= 7;
						unFrameSize += frhd[i + 4] & 0x7F;
					}
				}

				if(strcmp(szFrameId, "TPE1") == 0)
				{
					// artist
					if(Mp3ReadId3V2Text(pInFile, unFrameSize, pszArtist, unArtistSize) != 0)
					{
						break;
					}
					nFramesToRead--;
				}
				else if(strcmp(szFrameId, "TIT2") == 0)
				{
					// title
					if(Mp3ReadId3V2Text(pInFile, unFrameSize, pszTitle, unTitleSize) != 0)
					{
						break;
					}
					nFramesToRead--;
				}
				else
				{
					if(f_lseek(pInFile, f_tell(pInFile) + unFrameSize) != FR_OK)
					{
						return 1;
					}
				}
			}
		}
		if(f_lseek(pInFile, unSkip) != FR_OK)
		{
			return 1;
		}
	}

	return 0;
}




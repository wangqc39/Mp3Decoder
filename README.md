# Mp3Decoder
Decode MP3 file in the filesystem based on the FLASH. 

Helix library:
  Decoder the MP3 file to WAV.

NOR FLASH Driver:
  The data is stored in the NOR FLASH. 
  Support Winbond and GD FLASH. The supporting cound be extented.
  Very effective.

USB MassStorage:
  Transmit the MP3 file to the NOR FLASH.
  Get the WAV file from the NOR FLASH.
  PageSize is 4096 Bytes.
  
FAT FS:
  Visit the MP3 file and write the WAV file when decoding.
  
  
未增加Helix时，Code=19048  RO-data=580  RW-data=436  ZI-data=26916

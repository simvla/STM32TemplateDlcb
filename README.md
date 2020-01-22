## *Project description :*

Stm32f103xx HAL based template for DLCB and CodeBlocks IDE  
DLCB "data log and control board" is mini educational board for acquire,analyze and display data and control other devices  
DLCB is based on stm32 "bluepill" board with preloaded bootloader  
Contains tftLcd color 1.8 inch disp,three push buttons,status led and IO pin connectors   
Has a prepared toolchain to start develop immediately

## *Usage instructions :*

Download toolchain archive file(CBSTM32.rar) and extract to C:\CBSTM32  
[tool](http://www.mediafire.com/file/1up08iaxh97kk4d/CBSTM32.rar/file)  
On Windows go to command promt,switch to C:\CBSTM32\proj and clone STM32TemplateDlcb repo  
git clone https://github.com/simvla/STM32TemplateDlcb  
Download stm32 programmer and extract to C:\CBSTM32  
[prog](https://libstock.mikroe.com/projects/download/1862/10967/1531996605_mikrobootloader_other_other.zip)  
In CBSTM32 folder go to cbxx folder then open codeblocksSTM32  
on file menu click open... , navigate to proj/STM32TemplateDlcb, open STM32TemplateDlcb.cbp  
go to Build menu, click Rebuild,after build is complete go to Tools and click STM32-boot  
Insert usb cable on board and pc, wait for device id to appear, click connect  
Browse hex file(proj folder\bin\Release\STM32Template.hex) then click upload  

## *DLCB images :*  

![DLCB_pr_w1](https://user-images.githubusercontent.com/50554409/57645231-1ea6e180-75be-11e9-88b3-cb45adcaf1ad.png)  
img1-board mounted on front panel with printed mask  



for details about DLCB and software contact me on mail: vlada984@ymail.com




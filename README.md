## *Project description :*

Stm32f103xx HAL based template for DLCB and CodeBlocks IDE  
DLCB "data log and control board" is mini educational board for acquire,analyze and display data and control other devices  
DLCB is based on stm32 "bluepill" board with preloaded bootloader  
Contains tftLcd color 1.8 inch disp,three push buttons,status led and IO pin connectors   
Has a prepared toolchain to start develop immediately

## *Usage instructions :*

Download toolchain archive file(CBSTM32.rar) and extract to C:\CBSTM32  
[tool](http://www.mediafire.com/file/t6a8oxvx8ucszdj/CBSTM32.rar/file)  
On Windows go to command promt,switch to C:\CBSTM32\proj and clone stm32f103_ProjTemplate_cb repo  
git clone https://github.com/simvla/stm32f103xx_ProjTemplate_cb  
Download stm32 programmer and extract to C:\CBSTM32  
[prog](https://libstock.mikroe.com/projects/download/1862/10967/1531996605_mikrobootloader_other_other.zip)  
In CBSTM32 folder go to cbxx folder then open codeblocks  
on file menu click open... , navigate to proj/stm32f103xx_ProjTemplate_cb, open stm32f103xx_ProjTemplate_cb.cbp  
go to Build menu, click Rebuild,after build is complete go to Tools and click bootloader tool  
Insert usb cable on board and pc click connect, wait for device id to appear  
Browse hex file(proj folder\bin\Release\stm32f103_ProjTemplate_cb.hex) then click upload  

## *DLCB images :*  

![DLCB_pr_w1](https://user-images.githubusercontent.com/50554409/57645231-1ea6e180-75be-11e9-88b3-cb45adcaf1ad.png)  
img1-board mounted on front panel with printed mask  



for details about DLCB and software contact me on mail: vlada984@ymail.com




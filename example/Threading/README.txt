First, ensure that in the root directory of this example is a folder "dest"  
Then compile it by:  
  >> make  
Then, in another terminal, go to root dir of this example and call:  
  >> openocd  
Then, in this terminal, use:  
  >> make flash  
And, in the opened GDB console, type:  
  >> c  
And that's all. You should see LEDs blinking with different frequency.  
  
  
Pinout:  
Connect LED diodes with resistors to PA5, PA6 and PA7  

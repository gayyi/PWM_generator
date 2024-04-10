This project create a dual output PWM wave to control two mosfet and thus realize the open and closure of subinterval cicuits. The "main.c" and "pid.c" files are the file created by me. 

# Main topic
- This project creates the control signal of a dual terminal voltage converter.
- The exact description and requirement for the converter is as follows:
  ![image](https://github.com/gayyi/PWM_generator/assets/105962073/26139ca6-a0b0-4955-89c8-390ae9897d82)

# Files explanations
- The schematic figure for our original circuit is placed in "schematic" folder
- The executive code is in "Core/Src"

# Preset explanations
- The predetermined set is for *STM32F401RCT6* core unit
- We used one particular *TIM1* with two *supplementary* output channels to output two ways of supplementary *PWM* wave
- Other hardware components are recorded in *schematic.png* in *shematic* folder
  

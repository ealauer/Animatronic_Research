# Humanoid Animatronic Learning Simulator for Medical Interactive Training (H.A.L. S.M.I.T., or HAL for short)
Created by Ethan Lauer, Master of Science Robotics Engineering, Class of 2021, Worcester Polytechnic Institute (WPI)

Advised by Professor Pradeep Radhakrishnan, Department of Mechanical and Robotics Engineering, Worcester Polytechnic Institute (WPI)

Last Edited Spring 2021.

# Introduction
Medical simulation is becoming more prominent in the medical community as it provides a highly realistic and beneficial training experience. The technology is expensive for low-budget medical programs, and manikins have limited capacity for complex tasks. This project addressed such gaps by producing a low-cost head and neck animatronic prototype capable of performing complex simulations. The target simulations for the manikin included airway management, level of consciousness testing, and circulatory assessment. Major facial features were designed so the mechanical systems could exhibit anthropomorphic characteristics. Sensors were integrated into the design, allowing the animatronic to detect and react to changes in the surrounding environment. The subsystems were constructed and tested, however, challenges were faced during full assembly. Despite these challenges, the design demonstrates the potential for a similar application in the medical field. This GitHub repository contains the code, circuit diagrams, and CAD files for HAL.

# Initial Information
Be sure to install the latest version of Arduino (https://www.arduino.cc/en/software) and the Teensy Loader (https://www.pjrc.com/teensy/loader.html) before uploading the code.

Follow the steps detailed in the manual and note the comments in the official when working on H.A.L.

# Folder Descriptions
The folder titled "HAL" contains all of the Arduino code to control HAL.

The folder titled "MATLAB" contains the code and calculations for the possible Stewart platform neck design. This also includes the resulting plots and .xls files of the servo angles necessary for various movements.

The folder titled "New Neck Test" contains area where Arduino code could be developed for the Stewart platform.

The "WaveFilePlayerV1" folder contains the test file for playing pre-recorded audio from a microSD card in the Teensy using the Audio Library.

The folder titled "SolidWorks Files" contains the CAD files for the full system.


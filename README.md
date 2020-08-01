# Laser OS TouchDesigner

[TouchDesigner](https://derivative.ca/) is a node based programming environment for creative coding. This plugin provides driver support for LaserCube and LaserDock within TouchDesigner.

Currently Supported: Windows 64 bit

## Intro to TouchDesigner

TouchDesigner is a patch-based environment for creative coding. It has a unique workflow where you can visualize the signal at each step along the signal path. I would suggest getting aquainted with the basics of the environment before you try controlling lasers. Laser programming will primarily use the CHOP domain along with possibly SOP or TOP.

I recommend these two tutorials from Programming for People to start out.
- https://www.youtube.com/watch?v=SgA2BhF4w4o 
- https://www.youtube.com/watch?v=E4ZUzJK2ZQ4 

## Prerequisites

1) Download and install **[TouchDesigner](https://derivative.ca/download)**.
2) You need a LaserDock and ILDA laser, or a LaserCube - along with all appropriate cabling.
3) Be careful with your laser. You should know the procedures and equipment needed for safe operation.

## Get Started

1) Download the latest zip file from the **[releases page](https://github.com/Wickedlasers/Laser_OS_TouchDesigner/releases/)**
2) Unzip, and inside the folder, run the file Laser_OS_Test.toe
3) When TouchDesigner starts, you will see a security notice about loading the Laser OS CHOP plugin. Click 'OK' in order to load the plugin.
4) If you look around you will see the network that creates a circle inside a triangle, and causes the scene to rotate, and then output to the laser. In the lower right, look for the Laseros1 Operator and select it. You should see the properties window in the upper right, press P on the keyboard if you don't see it. Select your Laser OS device and use the toggle to Enable Laser Output when it is safe to do so.

![Enable Laser Output](http://prim8.net/files/Wickedlasers/enable_output.png)

## Installing the Plugin

You can run a TouchDesigner .toe file from the same location as the Laser OS **Plugins** directory, or you can install your plugins to use in all TouchDesigner projects on your computer.

To install on your system, copy the contents of **Plugins** into:

- Windows - Documents\Derivative\Plugins, usually C:\Users\<username>\Documents\Derivative\Plugins

For more information, please see: <https://docs.derivative.ca/Custom_Operators>

## Using the Plugin

Once the plugin is installed and approved to load, it will appear in the **OP Create Dialog** under the **Custom** tab.

![OP Create Dialog](http://prim8.net/files/Wickedlasers/op_create.png)

This operator expects one CHOP as an input, and that CHOP should have 5 channels. The names are not important, but they must be in the order: **X, Y, Red, Green Blue**. The sample rate of your channels should match the laser point rate you wish to send. 

Range of valid inputs are:
- X / Y: -1 to +1
- Red / Green / Blue: 0 to +1

You need a fairly fast computer to get good performance. If the laser is flickering when running TD in the editor, try switching to Perform mode to optimize performance.

**NOTE** there is a bug effecting Perform mode in some versions of TouchDesigner. If your laser does not output in Perform mode you can attach a Null CHOP to the output of Laseros1 and mark it as Cook Type: Always. More details about the bug are available on the [TD forum](https://forum.derivative.ca/t/fixed-c-cookeveryframe-and-perform-mode/141521).

![Perform mode workaround](http://prim8.net/files/Wickedlasers/cook_always.png)

## Learning Resources

There are many examples of different techniques available from <https://github.com/tgreiser/etherdream-touch-designer>. Any of these examples will be able to run on Laser OS with a minimal change. Add the Laser OS CHOP next to the ether_dream_controls, and wire it up from the same source. With this method, most examples for Ether-Dream or Helios DACs can be made compatible.

![Example](http://prim8.net/files/Wickedlasers/example.png)

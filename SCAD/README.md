# Lorize: a family of low profile 60% ergonomic keyboards that aren't too weird.  

These should be familiar to users of a MS natural or Sculpt keyboard, but pull in elements from the boardrun, alice, and kinesis keyboards. 
Available in desktop and MNT reform versions.

The lorize.scad file is intended to be used with the openscad configurator. You can change the type of keyboard (desktop, laptop, mutants), and the layer. The output can be used for laser cutting plate and case files. The POS files are used as an easy way to export centroid information from kicad & import into SCAD, which is used to make the bottom layer cutouts. 

lorize3 folder is for a desktop version of the PCB. it can be split for tenting, or used unsplit. This PCB can be soldered using only through hole 
techniques (no stencil).

The PCB for the laptop version is located at ![github milestogo/reform](https://github.com/milestogo/reform/blob/ergo1/reform2-keyboard-pcb/reform2-keyboard-ergo.jpg),in the ergo1 branch. This was done as a remix of the original keyboard, so that the factory firmware will still work. The board was updated to be single sided to reduce cost/effort of assembly. There isn't quite enough room to fit on top to fit a pro micro, so it requires SMD work. 


#Todos.
the latest ergo reform PCB thumb positions are a few mm away from the scad positions. Update. 

# gluexdirc
##Synopsis
```
glxdirc [OPTION] [ARGUMENT] ... [OPTION] [ARGUMENT]

example:
./glxdirc -s 0 -l 3 -wg 1. -e 1 -x muon -p 4. -z -2 -gx 225. -gy 268.

```
##Options
```
-o    output file name
-i    input file name
-u    look-up file name

-s    run type
                0    simulation
                1    look-up table generation
                2    reconstruction

-g    geometry configuration
                0    single EV tank
				1    two EV tanks - default value

-c   MCP layout
		0    one MCP cover all FD plain (custom pixel size)
        2    standard MCPs (6.4x6.4 pixels) - default value
		

-mr   radius of the focusing mirror (default value 800mm)

-mt   tilt of the focusing mirror (default value 16 degrees)

-l	  focusing type
		0   cylindrical mirror 
		n   n-segmented mirror (usually n = 3)

-dt   tilt of the photodetection plane

-wg   thickness of the (air) gap between the window and the photosensors
	check the material of the gap!!!

-gx   x position of the hit in the radiator [-2490,2390] [mm] 

-gy   y position of the hit in the radiator [160,1000]   [mm]

-phi  athimuthal angle of the charge track [0,360] degree
 
-theta polar angle of the charge track [0,11] degree

-z    track direction
                -1 random track (always hits the radiator) 
                -2 use gx, gy to calculate track angle
		
-e    number of simulated events

-x    particle type
              "pi+" 
              "proton"
              "kaon"
                 ...
              "opticalphoton"   

-p    particle momentum [GeV/c]

-w    physics list
                0    standard (default value)
                1    without multiple scattering
                10   monochromatic Cherenkov light
                11   10 + 1 

-r    seed number for the random generator 

-h    bar id for the LUT simulation [0,47]

-b    batch mode
               1    run silent (without GUI)

```

##Examples
```

lut generation for bar #0 (-h 0 -> l_b0.root):
./glxdirc -o l_b0.root -e 1000000 -s 1 -x opticalphoton -p "3.18 eV" -c 2 -g 1 -w 0 -wg 0  -h 0 -b 1

adding look-up tables for all bars:
root glxlut_add.C'("l_b*.root","lut_all.root")'

simulation:
./glxdirc -s 0 -e 100 -x proton -p 4.5 -z -2 -gx 225 -gy 268 -b 1

reconstruction:
./glxdirc -s 2 -u lut_all.root -i hits.root -f -1 -t1 10. -e 0 -v 4
or 
./glxdirc -s 2 -u lut_all.root -i hits.root -f 0 -t1 10. -e 0 -v 2


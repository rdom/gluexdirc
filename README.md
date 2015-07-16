# gluexdirc
##Synopsis
```
glxdirc [OPTION] [ARGUMENT] ... [OPTION] [ARGUMENT]

example:
./glxdirc -s 0 -e 0 -h 1 -s 0 -x "K+" -p 10 -z -2 -gx 2000 -gz 200

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
                0    default

-c   MCP layout
                0    4x6 standard MCPs (pixel size == mcp size)
                1    4x6 standard MCPs (6.4x6.4 pixels)
		3    one MCP cover all FD plain (custom pixel size)
		4    4x6 MCPs with compact paking and 32x32 pixels (2 mm x 2 mm)

-mr   radius of the focusing mirror

-mt   tilt of the focusing mirror

-dt   tilt of the photon-detection plane

-gx   x position of the hit in the radiator [-2490,2390] [mm] 

-gy   y position of the hit in the radiator [160,1000]   [mm]

-e    number of simulated events

-x    particle type
              "pi+" 
              "proton"
              "kaon"
                 ...
              "opticalphoton"   

-p    particle momentum [GeV/c]

-w    physical list
                0    standard
                1    without multiple scattering
                10   monochromatic Cherenkov light
                11   10 + 1 

-r    seed number for the random generator 

-b    batch mode
               1    run silent (without GUI)

//-d    display option
               0    standard (default)
               1    display hit occupancy of current run
               2    display hit occupancy of occuhits.root (need to be generated)
```

##Example of script usage from macro folder
```


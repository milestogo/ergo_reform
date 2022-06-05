# cd $PATH/to/lorize_kb
# ./export-scad-centroids.sh 
# in kicad console
# exec(open("scad-to-kicad.py").read())
import pcbnew
import csv

pcb = pcbnew.GetBoard()

# mm to nm
MMSCALE=1000000.0
DEGSCALE=10.0 # 10th of degree


with open("foo.echo") as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
                SW=row[3][2:-1]
                DI=row[7][2:-1]
                LED=row[11][2:-1]

                switch =  pcb.FindFootprintByReference(SW)
                
                # print(type(mod),bool(mod), str(mod))
                if switch:
                        myX=float(row[4])
                        myY=float(row[5])
                        myA=float(row[6])
#                        print(SW, myX, myY, myA)

                        switch.SetPosition(pcbnew.wxPoint(myX * MMSCALE , myY * MMSCALE))
                        switch.SetOrientation( myA * DEGSCALE  )

                diode =  pcb.FindFootprintByReference(DI)
        
                if diode:
                        myX=float(row[8])
                        myY=float(row[9])
                        myA=float(row[10])
#                        print(DI, myX, myY, myA)

                        diode.SetPosition(pcbnew.wxPoint(myX * MMSCALE , myY * MMSCALE))
                        diode.SetOrientation( myA * DEGSCALE  )


                led =  pcb.FindFootprintByReference(LED)
        
                if led:
                        myX=float(row[12])
                        myY=float(row[13])
                        myA=float(row[14])
#                        print(LED, myX, myY, myA)

                        led.SetPosition(pcbnew.wxPoint(myX * MMSCALE , myY * MMSCALE))
                        led.SetOrientation( myA * DEGSCALE  )



#pcb.Save('reform2-keyboard-scripted.kicad_pcb')
pcbnew.Refresh()

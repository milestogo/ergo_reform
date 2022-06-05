include <part-POS.scad>;
include <lorize-desktop.scad>;
include <lorize-keyfuncs.scad>;


$fn = 30;

angle = 14; // Total opening angle
splita = angle / 2;
colstep = 18.75;   // horizontal center to center distance between switches
rowstep = 17.1; // ibid, vertical
m3 = 3.1 / 2;   // radius of m3 hole
centerrow=2; // which row is 0 in the middle of a column. 
/* [Hole Type:] */
// Pick layer to set hole type. 0 = debug, 1 = switchplate
layer = 0; // [0:caps_debug, 1:switchplate, 2:pcb_frame, 3:pcb_shim, 4:palm_mid,
           // 5: palm_rest]


/* [ Board Type:] */
board = 0; // [ 1:maxreform, 2:reform, 0:desktop, 3:smallreform_thumb]



module rtate(x, y, a) {
  newx = x * cos(a) - y * sin(a);
  newy = x * sin(a) + y * cos(a);
}

///  PCB Size 
reform_pcbx = 275.9;
reform_pcby = 102.24;

// mcu board size
mcu_x = 33;   // pro micro 33, feather 51, bluepill 53, weact blackpill 52
mcu_y = 19; // pro micro 19 /bluepill 22.6, weact blackpill 21, roboyn blackpill 18.5 / feather 23
mcu_z = 8;



/// values used to match board types 
desktop = 0;           // 60% best overall ergos, custom PCB shape.
maxreform = 1;         // option with as many keys as we can cram in.
reform = 2;            // 60% that fits well enough for a reform sized PCB
smallreform_thumb = 3; // 30% option with more thumbs for reform.
long = 1;              // used to flag if switch has long or standard cap.

// enum of values used for cdomparison. 
caps_debug = 0;
switchplate = 1;
pcb_frame = 2;
palm_mid = 4;
palm_rest = 5;


// lookup table used for the debugkey function to map col, row to a footprint name in kicad. 
switch_layout = [ // layout for switches & diodes
[ "72",   "5", "11","17", "23", "29", "35", "41", "47", "53", "59", "65", "71","73"], // top row, row4
[ "",   "4", "10","16", "22", "28", "34", "40", "46", "52", "58", "64", "70"],
[ "",   "3", "9", "15", "21", "27", "33", "39", "45", "51", "57", "63", "69"],
[ "",   "2", "8", "14", "20", "26", "32", "38", "44", "50", "56", "62", "68"], // row 1
[ "", "1", "7", "13", "19", "25", "31", "37", "43", "49", "55", "61", "67"], // row 0
];

led_layout = [ // layout for LEDs, which is based on contiguous verticals
[ "",   "84", "93","94", "103", "104", "113", "114", "123", "134", "143", "144", "153"], // top row, row4
[ "",   "85", "92","95", "102", "105", "112", "115", "122", "135", "142", "145", "152"],
[ "",   "86", "91", "96", "101", "106", "111", "116", "121", "136", "141", "146", "151"],
[ "",   "87", "90", "97", "100", "107", "110", "117", "120", "137", "140", "147", "150"], // row 1
[ "",   "88", "89", "98", "99", "108", "109", "118", "119", "138", "139", "148", "149"], // 
];




//// Functions


module do_both_sides(angles, vfudge, hfudge, rowlow, rowhi, separation) {
  for (m=[1,0]) {
    mirror([ m, 0, 0 ])
      doright(angles, vfudge, hfudge, rowlow, rowhi, separation, m);
  }
}

module do_reform_pcb() {

  union() {
    square([ reform_pcbx, reform_pcby ], center = true);
    translate([ 0, +10.92 ])
        square([ reform_pcbx - 2 * 33.510, reform_pcby ], center = true);
  }
}


//  Punch out one half of the shape.
module doright(angles, vfudge, hfudge, rowlow, rowhi, separation, is_mirror) {
  for (x = [1:5]) { // starting with middle finger towards pinky, 

    for (y = [rowlow[x]:rowhi[x]]) {
      debugkey((x - 2) * colstep, 0, splita, hfudge[x], vfudge[x] + (y)*rowstep,
               angles[x], separation, 0, 0, is_mirror, x,y );
    }

    // for cols 5 & 6 we add non-matrix keys. 
    if (5 == x) { // pinky shift key is special.
      if (board ==  smallreform_thumb) { // why can't the laptop have a big key too?
        debugkey(1 + (x - 2) * colstep, 0, splita, -0.5,
                 vfudge[x] + (-1) * rowstep, angles[x], separation, 0, 0,
                 is_mirror,x,-1);
      } else {
      // shiftkey
       debugkey((x - 2 + 0.3) * colstep, 0, splita, -0.75,
                 vfudge[x] - 0.2+ (-1) * rowstep, angles[x], separation, 1, 0,
                 is_mirror,x,-1);
      }

      // pinky menu / long key
      // small reform uses 1u keys, mostly for aesthetics. others use
      // larger 1.5U
      if (board != smallreform_thumb) {
     	debugkey((colstep * 2.25),0 , splita, 0.5,
                   vfudge[x] - 2 * rowstep -0.1, angles[x], separation, 1,
                 0, is_mirror,4,-centerrow);
      }

      // pinky ctrl key
      if ((board == reform) || board == desktop) {
        debugkey((3.5) * colstep, 0, splita, 0,
                 -1.5 - vfudge[x] - (2) * rowstep, angles[x], separation, 0, 0,
                 is_mirror, x, -centerrow);
      }
    }
  }

  // x=0 index fingers have extra rotation to bring up spacebar y location.
  for (x = 0) {
    for (y = [rowlow[x]:rowhi[x]]) {
      debugkey((x - 2) * colstep, 0, splita, hfudge[x], vfudge[x] + (y)*rowstep,
               angles[x], separation, 0, -splita * 0.55, is_mirror, x, y);
    }
  }

  // Far reach upper pinky keys
  if ((board == maxreform) || (board == reform)) {

    for (x = 6) { // weird pinky keys
      if (board == maxreform || (board == reform)) {
        for (y = [rowlow[x]]) {
          debugkey((x - 2) * colstep, 0, splita,  hfudge[x]- 0.75,
                   vfudge[x] + (y -0.15) * rowstep, angles[x], separation, 0, 90,
                   is_mirror, x,-centerrow);
        }
      }
    }
  }
}


module reform_thumbs(separation,yshift) {

/* /// Inputs x_origin,y_origin, angle of orgin from horizontal, x increment ,y increment,
/// additional angle of column, separation between hands, do_longkey,
/// per_key_rotation, mirror key, col,row
*/

  // inner / proximal / index side
  //echo ("right");
    // less than.5mm below bottom of PCB edge, 
    // X center as close as possible to line of index finger keys. 
*   debugkey(-2 * colstep, -2.5 * rowstep, 0, -1, -1.5-yshift, 0, separation, 0, 90, m, 0,-centerrow );
 debugkey(-2 * colstep, -2.5 * rowstep, 0, 3, -yshift, 0, separation, 0, 110, 0, 0,-centerrow );


  // distal / pinky side
  // rotate inner key 90% because that lets us bring it 2mm closer to pinky side key. 
    // X center as close as possible to line between index keys. 
    // Y as high as possible to allow thumb to hit inner key. 
*    debugkey(-1 * colstep, -2 * rowstep, 0, 1.5-(rowstep-14), -4-yshift, 0, separation, 0, splita-1.25,1, 1,-centerrow);

debugkey(-1 * colstep, -2 * rowstep, 0, 4.5-(rowstep-14), -5-yshift, 0, separation, 0, 110,0, 1,-centerrow);


mirror([1,0,0]) {
  //echo ("Left");
  // inner / proximal / index side
    // less than.5mm below bottom of PCB edge, 
    // X center as close as possible to line of index finger keys. 
*   debugkey(-2 * colstep, -2.5 * rowstep, 0, -1, -1.5-yshift, 0, separation, 0, 90, m, 0,-centerrow );
 debugkey(-2 * colstep, -2.5 * rowstep, 0, 3, -yshift, 0, separation, 0, 110, 1, 0,-centerrow );


  // distal / pinky side
  // rotate inner key 90% because that lets us bring it 2mm closer to pinky side key. 
    // X center as close as possible to line between index keys. 
    // Y as high as possible to allow thumb to hit inner key. 
*    debugkey(-1 * colstep, -2 * rowstep, 0, 1.5-(rowstep-14), -4-yshift, 0, separation, , splita-1.25,1, 1,-centerrow);

debugkey(-1 * colstep, -2 * rowstep, 0, 4.5-(rowstep-14), -5-yshift, 0, separation, 1, 110,1, 1,-centerrow);

}

 }


/*
module desktop_thumbs(separation, yshift1, yshiftL) {

  // thumbs
    debugkey(-2 * colstep, -2.5 * rowstep, 0, -0.75, yshiftL, 0, separation,
             long, 0, 1);

    debugkey(-0.75 * colstep, -2 * rowstep, 0, -1.5, yshift1, 0, separation, 0,
             0, 1);

  mirror([ 1, 0, 0 ]) {
    debugkey(-2 * colstep, -2.5 * rowstep, 0, -0.75, yshiftL, 0, separation, long, 0, 1);

    debugkey(-0.75 * colstep, -2 * rowstep, 0, -1.5, yshift1, 0, separation, 0,
             0, 1);
  }
 }
*/


///////// Main per-board descriptions. 
// Supply the SW or D part names to kicad. Different matrixes will have different layout. 

if (board == reform) {
  /* 60 % board, with 1.5 and 1u keys, thumbs aren't perfect, but work */



echo("Reform Debug","col", "row",  "Switch", "X", "Y", "angle", "Diode", "DiodeX", "DiodeY", "DiodeAngle", "LED", "LedX","Led Y", "Led_A");


  angles = [ -1.5, -1.5, -0.75, 0, -0.75, -1.5, -1.5, -1.5 ]; // inner to outer(pinky)
  vfudge = [ 2.5, 1, 0, 0, 0, -0.5, -0.5 ];         // inner to outer
  hfudge = [ -0.5, -0.5, -.33, 0, 0, .6, 0.25 ];

  rowlow = [ -1, -1, -2, -2, -1, 0, 2 ];
  rowhi = [ 2, 2, 2, 2, 2, 2, 2 ];

  pcbx = reform_pcbx;
  pcby = reform_pcby;
  pcb_y_shift = -1; // manually move center of board up/down in relation to center of switches. 

  difference() {
    translate([ 0, pcb_y_shift, 0 ]) do_reform_pcb();

    separation = 53;

    do_both_sides(angles, vfudge, hfudge, rowlow, rowhi, separation);

    // thumbs

    reform_thumbs(separation, pcb_y_shift);

   * translate ([0,-1,0])
    		do_mcu(0);
  }
}



// Max reform started as an attempt to add palm keys & some extra switch locations. 
if (board == maxreform) {

  angles = [ -1.5, -1.5, -0.5, 0, 0, -1.5, -1.5 ]; // inner to outer(pinky)
  vfudge = [ 2.5, 1, 0, 0, 0, -0.5, 0 ];           // inner to outer
  hfudge = [ -1.25, -0.75, -.33, 0, 0, .6, 0.25 ];

  rowlow = [ -1, -1, -2, -2, -1, 0, 1 ];
  rowhi = [ 2, 2, 2, 2, 2, 1, 1 ];

  separation = 53;
  pcbx = reform_pcbx;
  pcby = reform_pcby;
  pcb_y_shift = 0;

  difference() {
    translate([ 0, pcb_y_shift, 0 ]) do_reform_pcb();

    do_both_sides(angles, vfudge, hfudge, rowlow, rowhi, separation);

    /* put key in upper corners
    translate([(pcbx-colstep)/2,(pcby-rowstep)/2,0]) {
            dokey();
            translate([0,-rowstep,0])
                    dokey();
    }

    */

    desktop_thumbs(separation, -2.5, 3.5);
  }
}

//  Small Reform - 40% board with extra room for thumb keys. 
// it's still not enough Y axis room. 
if (board == smallreform_thumb) { //  power button on left, LCD goes on right.
  /* 40% board, with 1u keys, more thumb buttons */

  q = -0.3; // relax angles because of wider spacing, or you could increase,
            // there's room.
  angles = [ -1.5, -1.5, -0.75, 0, 0, -1.5, -1.5 ]; // inner to outer(pinky)
  vfudge = [ 2.5, 1, 0, 0, 0, -0.5, -2 ];           // inner to outer
  hfudge = [ -1.25, -0.75, -.33, 0, 0, .6, 0.25 ];

  rowlow = [ -1, -2, -2, -2, -2, 0, 0 ];
  rowhi = [ 1, 1, 1, 1, 1, 1, 1 ];

  pcbx = reform_pcbx;
  pcby = reform_pcby;
  pcb_y_shift = 15;

  separation = 53 + 4.75; // probably best with angle = 13?

  difference() {
    translate([ 0, -pcb_y_shift, 0 ]) translate([ 0, pcb_y_shift, 0 ])
        do_reform_pcb();

    translate([ 0, pcb_y_shift, 0 ])
        do_both_sides(angles, vfudge, hfudge, rowlow, rowhi, separation);

    for (y = [ 3, 4 ]) {
      translate([ 0, -pcby / 2 - pcb_y_shift + (y)*rowstep, 0 ]) { dokey(); }
    }

    translate([ 0, -pcby / 2 - pcb_y_shift + rowstep / 2, 0 ]) { dolongkey(); }

    // thumbs
    translate([ 0 * colstep, -2.35 * rowstep, 0 ]) dolongkey();
    for (x = [ -1, 1 ])
      translate([ x * 1.25 * colstep - x * 0.5, -2.0 * rowstep, 0 ]) dokey();
  }
}

if (board == desktop) {

  angles =
      [ -1.5, -1.5, -.75, 0, 0, -1.5, -1.5 ]; // inner index to outer(pinky)
  vfudge = [ 2, 1, 0, 0, 0, -0.5, -0.5 ];     // inner to outer
  hfudge = [ -0.5, -0.5, -.33, 0, 0, .6, 0.25 ];

  rowlow = [ -1, -1, -2, -2, -1, 0, 1 ];
  rowhi = [ 2, 2, 2, 2, 2, 2, 1 ];
  separation = 53;
  pcb_y_shift = 0;


echo("Desktop Debug", "X", "Y", "angle", "col", "row", "kicad switch location", "kicad LED", "kicad diode");

  if (layer == 1) { // switchplate

    difference() {
      hullaplate(1);
      translate([ 0, pcb_y_shift, 0 ])
          do_both_sides(angles, vfudge, hfudge, rowlow, rowhi, separation);

      desktop_thumbs(separation, -2.5, -2.5);

      // pins under MCUs
      for (i = [0]) {
        translate([ mcus[i][0], mcus[i][1], 0 ]) rotate(mcus[i][2])
            do_mcu_under();
      }
    }
  } else if (layer == 2) { // pcb pcb_frame
    difference() {
      union() {
        hullaplate(1);
        hullpalms();
      }

      // diode cutouts
      for (i = [0:59]) {
        translate([ diodes[i][0], diodes[i][1], 0 ]) rotate(diodes[i][2])
            hull() for (x = [ -5 + 1, 5 - 1 ]) translate([ x, 0, 0 ])
                circle(r = 3 / 2);
      }

      // resistor cutouts
      for (i = [0:7]) {
        translate([ resistors[i][0], resistors[i][1], 0 ])
            rotate(resistors[i][2]) hull() for (x = [ -5 + 1, 5 - 1 ])
                translate([ x, 0, 0 ]) circle(r = 3 / 2);
      }

      // make choc under footprints
      translate([ 0, pcb_y_shift, 0 ])
          do_both_sides(angles, vfudge, hfudge, rowlow, rowhi, separation);

      desktop_thumbs(separation, -2.5, -2.5);

      // pins under MCUs
      for (i = [ 0, 1 ]) {
        translate([ mcus[i][0], mcus[i][1], 0 ]) rotate(mcus[i][2])
            do_mcu_under();
      }

      // free  hand a place to do RGB lighting

      hull() {
        translate([ 4.5, -31 ]) circle(r = 3);

        translate([ -4.5, -31 ]) circle(r = 3);

        translate([ 0, -47 ]) circle(r = 3);
      }
    }

  } else if (layer == palm_mid) {

    hullpalms_mid();

  } else if (layer == palm_rest) {
    hullpalms_top();

  } else {
    difference() {
      hullaplate(1);

      translate([ 0, pcb_y_shift, 0 ])
          do_both_sides(angles, vfudge, hfudge, rowlow, rowhi, separation);

      desktop_thumbs(separation, -3, -2.5);

      /*
                              // asymmetric hand keys
                      for (x=[6]) { // far right pinky
                                      for (y=[1,2])
                                              debugkey ( (x-2)*colstep, 0 ,
         0.5*splita , -1+hfudge[x], 3+ vfudge[x]+(+y)*rowstep, angles[x],
         separation,0, 0,"x","y");
                              }


                              mirror([1,0,0])
                              for (x=[6]) { // far right pinky
                                      for (y=[1,2])
                                              debugkey ( (x-2)*colstep, 0 ,
         0.5*splita , -1+hfudge[x], 3+ vfudge[x]+(+y)*rowstep, angles[x],
         separation,0, 0,"x", "y");
                              }
      */
    }
  }
}

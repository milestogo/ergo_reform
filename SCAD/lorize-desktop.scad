/* Desktop specific configs for lorize keyboard */



// desktop shape functions.

module walkpoints(radius, start, count, nodes) {
  for (c = [start:count]) {
    translate(nodes[c]) circle(r = radius);
  }
}

RightCorners = [
  [ 0, 0 ],
  [ 0, 58 ],     // center top
  [ 62, 64 ],    //  outsidetop of mcu holder 192,36
  [ 72, 48 ],    // near u key // 205,49.5
  [ 112.5, 49 ], // upper right
  [ 125, 3 ],    // bump on side
  [ 129, -35 ],  // bottom right
  [ 27, -53 ],   // side of spacebar
  [ 0, -53 ],    // bottom center
  [ 0, 0 ],
];

/*raw, center at x=130,y=100
130,42
130,100
130, 153
200,49
60,49
254,98
6,98
148,135
112,135
*/

MountingHoles = [
  [ 0, 58 ],
  [ 0, 0 ],
  [ 0, -53 ],
  [ 70, 51 ],
  [ -70, 51 ],
  [ -124, 2 ],
  [ 124, 2 ],
  [ 18, -36 ],
  [ -18, -35 ],
];

front = 140;
gap = -20;
PalmrestMid = [
  // middle ones, no gap added to allow access to keys
  [ 142, 10 - front ],
  [ 129, -35 ], // bottom right
  [ 27, -53 ],  // side of spacebar
  [ 0, -53 ],   // bottom center
  [ 40, 5 - front ],
  [ 00, 5 - front ],
];

PalmrestTop = [
  // middle ones, no gap added to allow access to keys
  [ 142, 10 - front ],
  [ 129, -35 ], // bottom right
  [ 40, -53 ],  // side of spacebar
  [ 70, -53 ],  // bottom center
  [ 50, 5 - front ],

];

LowerPalmrestCorners = [
  [ 145, 10 - front ],
  [ 129, -35 ], // bottom right
  [ 27, -53 ],  // side of spacebar
  [ 0, -53 ],   // bottom center
  [ 50, -front ],
  [ 0, -front ],

];

module hullpalms_mid() { // upper palms
                         // no fancy angles, so we can do both sides.
  difference() {
    union() {
      hull() walkpoints(5, 0, 5, PalmrestMid);

      mirror([ 1, 0, 0 ]) hull() walkpoints(5, 0, 5, PalmrestMid);
    }

    scale(1.01) hullaplate();

    // centering holes for palmrest.
    for (x = [ 100, 80, -80, -100 ])
      translate([ x, -100, 0 ]) circle(r = 3 / 2);
  }
}

module hullpalms_top() { // upper palms
                         // no fancy angles, so we can do both sides.
  difference() {
    union() {
      hull() walkpoints(5, 0, 4, PalmrestTop);

      *mirror([ 1, 0, 0 ]) hull() walkpoints(5, 0, 5, PalmrestTop);
    }

    translate([ 0, -18, 0 ]) scale(1.05) hullaplate(0);

    translate([ 0, -150 ]) rotate(45) square([ 150, 150 ]);

    // centering holes for palmrest.
    for (x = [ 100, 80 ])
      translate([ x, -100, 0 ]) circle(r = 3 / 2);
  }
}

module hullpalms() {
  // no fancy angles, so we can do both sides.
  difference() {
    union() {
      hull() walkpoints(5, 0, 5, LowerPalmrestCorners);

      mirror([ 1, 0, 0 ]) hull() walkpoints(5, 0, 5, LowerPalmrestCorners);
    }
    // always cut out mounting holes
    for (c = [0:8]) {
      translate(MountingHoles[c]) circle(r = m3);
    }

    // centering holes for palmrest.
    for (x = [ 100, 80, -80, -100 ])
      translate([ x, -100, 0 ]) circle(r = 3 / 2);
  }
}

module hullaplate(doholes) {
  difference() {
    union() { // need to do two different hull points.
      hull() walkpoints(5, 0, 3, RightCorners);
      hull() walkpoints(5, 3, 9, RightCorners);

      mirror([ 1, 0, 0 ]) {
        hull() walkpoints(5, 0, 3, RightCorners);
        hull() walkpoints(5, 3, 9, RightCorners);
      }
    }

    if (doholes) {
      // always cut out mounting holes
      for (c = [0:8]) {
        translate(MountingHoles[c]) circle(r = m3);
      }
    }
  }
}

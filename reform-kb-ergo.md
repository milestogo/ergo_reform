

## Design goals & trade-offs:

# Make it hurt less
Remove as many of the un-ergonomic factors as possible, while remaining within the footprint of the Reform keyboard.

# Optimize for portable use on the lap.
People will put their laptops at different heights and rotations. Most of these won't be ergonomic. We'll try to avoid anything that makes it worse.

While we can increase the hand to hand spacing up to almost 4cm, this moves the palm resting position out to the side enough that it unbalances a laptop when only one hand is used. Instead we're increasing the angle & keeping the hands relatively close. This normally requires an increase in tenting angle, which we're appoximating by changing the pinky location. 

Most designs here separate the D ahd K middle fingers by ~106mm. Most ergo keyboards are 2-3 cm further apart, but tip the laptop.  From a trawl through anthropometric data, people with forearms of average North American size should have a ~18 degree angle, but we're limited to  ~14 degrees in order to stay within the limits of the reform keyboard Y depth. Too flat is probably less bad than too angled given the range of positions a laptop will be placed in. 

best angle = atan((shoulders width -d_to_k_key_spacing)/( 2 * armlen));

#Minimize learning curve
  * Removing the number row is a big step for most people.  Providing a comfortable 10key layer will help some people, this is down to the key map. 
  * Removing the Fkeys is bad for some, but is easier to solve with a FN key/keymap. 
  * Use big keys where people are used to big keys. Most designs use 1.5U keys for shift & GUI, to accomodate current muscle memory.  
  
# Avoid custom keycaps
* We want to only use 1u and 1.5u keycaps. In practice, the top right and top left key may need to be shaved down to clear the MNNT bezel. The thumb buttons would benefit from a more diagonal shape. 

# Reduce ulnar flexion/deviation
Ulnar flexion occurs when the fingers are not in line with the bones of the forearm, so that there is a <180 degree angle between little finger and the outside of the arm). 

Ulnar flexion is the norm on laptop keyboards, where the hands are too close together and the keyboard is entirely parallel to the chest.

To fix this, we want the front to back line of keys to be  parallel to the bones of the forearm. This is changes depending on how far apart the hands are (x axis) as this changes the angle between the forearms and the chest.  Reducing pronation of the hands (thumbs to ceiling) allows the fingers to move together. Staggering keys so that the little finger is further away and the index finger is closer has the effect of rotating the center of the palm around the z-axis, reducing ulnar flexion. This technique has a limited range, beause people have different size fingers. 

Because of the limited depth of the reform keyboard, we can't use much finger stagger, but we've added some.  

# Minimize Radial Deviation
Radial flexion is caused by roating the hand inwards, as in rotating the palm to allow the little finger to hit the backspace key,  or to hit a key inwards of the index finger.  Laptop keyboards start with so much ulnar deviation, radial movement usually isn't a problem. Some variations here (smallreform_thumbs) add a column of keys between the index fingers. The lower positions are not good for radial deviation when activated by the fingers, and create ulnar deviation when activated by the thumb. Save them for unused keys.

#Reduce pronation of the hand
Pronation is palms forced parallel to the floor, when they would be more comfortable with the thumbs pointing upwards.

We gain a litle bit here by straightening the ulnar deviation, but proper tenting would require changing the top face of the laptop.

# Move work from the little finger to the Thumbs.
 This helps avoid "emacs-pinky", and somewhat reduces the need to pronate the hand in chorded combinations. Frequently used keys like Backspace, GUI, CTL and Enter should be moved to thumb keys.

 Some people have flexible or long thumbs that allow them to hit keys underneath the index fingers. Most people use excessive wrist flexion to force it.  

 There are two thumb keys on each hand in all designs. In the smallreform_thumbs version has 5 in total, with slightly worse ergonomics. While the bottom index finger key could be used for an inner thumb key, but I suspect it is bad for most people. 

# Avoid Wrist extension
Where the tip of the fingers are above the line from elbow to wrist. Laptops create a problem when the user is pressing down on the palms to keep the laptop steady.  Possibly some thin (~3-5mm), cork or rubber  wrist wrests could help wrist extension & contribute a degree or two of tenting?

# Think about palmar flexion
Where the tip of the fingers are below the line from the elbow to the wrists. Also known as the T-rex or bunny position. This can be a problem for tall laptops on tables, where there isn't a good line from the elbows to the keys. Ideally we'd move the laptop to the lap or use a separate keyboard.

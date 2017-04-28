//Scene 1 for final

camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1, 1, 1>}

//--------------Back Triangles:-----------------------
//Top Top
triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <1, 0, 0>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  translate <-12, 8, -10>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <0, 1, 1>}
  finish {ambient .3, diffuse .2}
  scale <2, 4, 2>
  translate <-4, 8, -10>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <.5, 0, 1.0>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  translate <4, 8, -10>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <1, 1, 0>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  translate <12, 8, -10>
}

//Top Bottom
triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <.5, .9, .2>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  rotate <0, 0, 180>
  translate <-12, 8, -10>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <.85, .1, .43>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  rotate <0, 0, 180>
  translate <-4, 8, -10>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <1, .5, 0>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  rotate <0, 0, 180>
  translate <4, 8, -10>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <0, 0, 1>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  rotate <0, 0, 180>
  translate <12, 8, -10>
}

//Bottom Top
triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <0, .5, 1>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  translate <-12, 0, -10>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <.5, 1, 0>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  translate <-4, 0, -10>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <0, 1, .5>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  translate <4, 0, -10>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <1.0, 0, .5>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  translate <12, 0, -10>
}

//Bottom bottom
triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <1, .66, .33>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  rotate <0, 0, 180>
  translate <-12, 0, -10>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <.33, .66, 1>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  rotate <0, 0, 180>
  translate <-4, 0, -10>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <1, .33, .66>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  rotate <0, 0, 180>
  translate <4, 0, -10>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <0, 1, 0>}
  finish {ambient .3, diffuse 1.0}
  scale <2, 4, 2>
  rotate <0, 0, 180>
  translate <12, 0, -10>
}

//sphere { <0, 1, 0>, 2
  pigment { color rgb <0, 0, 1.0> }
  finish { ambient 0.02 diffuse .8}
}

//-------------Refractive Spheres --------------------------------
sphere { <-6, 3, 0>, 1
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior 1.75 }
}

sphere { <-6.25, -2, -3>, 1
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior 1.75 }
}

sphere { <-3, 0, 2>, 1
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior 1.75 }
}

sphere { <.5, 4, 4>, 1
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior 1.75 }
}

sphere { <5, -3, 0>, 1
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior 1.25 }
}

sphere { <6, 2, -5>, 1
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior 1.9 }
}

sphere { <.2, -1, 0>, 1
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior 1.44 }
}

sphere { <-3.5, 7, -6>, 1
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior 1.25 }
}

sphere { <0, 1.5, 5>, 1
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior 1.95 }
}

sphere { <5, 4, 0>, 1
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior 1.5 }
}

plane {<0, 0, 1>, -50
  pigment {color rgb <0.1, 0.1, 0.1>}
  finish {ambient 0.3 diffuse 0.8}
}

plane {<0, 1, 0>, -4.9
  pigment {color rgb <1, 1, 1>}
  finish {ambient 0.3 diffuse 0.8 refraction 1.0 ior 1.5}
}

//Floor
plane {<0, 1, 0>, -5
  pigment {color rgb <0.9, 0.9, 0.9>}
  finish {ambient 0.3 diffuse 0.8 reflection .75}
}
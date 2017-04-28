//Scene 2 for final

camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source{<50, 100, 100> color rgb <1.0, 1.0, 1.0>}

//Numbers
//4
box { <-1, -1, -1>, <1, 1, 1>
  pigment { color rgb <0.3, 0.3, 0.8>}
  finish {ambient 0.2, diffuse 0.7}
  scale <.25, 1.5, .25>
  rotate <0, 45, 0>
  translate <-5, 2.25, 5>
}

box { <-1, -1, -1>, <1, 1, 1>
  pigment { color rgb <0.3, 0.3, 0.8>}
  finish {ambient 0.2, diffuse 0.7}
  scale <1.5, .25, .25>
  rotate <0, 45, 0>
  translate <-4, 1.1, 4>
}

box { <-1, -1, -1>, <1, 1, 1>
  pigment { color rgb <0.3, 0.3, 0.8>}
  finish {ambient 0.2, diffuse 0.7}
  scale <.25, 4, .25>
  rotate <0, 45, 0>
  translate <-3.25, 0.5, .5>
}

//7
box { <-1, -1, -1>, <1, 1, 1>
  pigment { color rgb <0.3, 0.3, 0.8>}
  finish {ambient 0.2, diffuse 0.7}
  scale <1.5, .25, .25>
  rotate <0, 45, 0>
  translate <-1, 4, 0>
}

box { <-1, -1, -1>, <1, 1, 1>
  pigment { color rgb <0.3, 0.3, 0.8>}
  finish {ambient 0.2, diffuse 0.7}
  scale <.25, 4, .25>
  rotate <0, 45, 0>
  translate <-0.1, 0.25, -.75>
}

//3
box { <-1, -1, -1>, <1, 1, 1>
  pigment { color rgb <0.3, 0.3, 0.8>}
  finish {ambient 0.2, diffuse 0.7}
  scale <1.5, .25, .25>
  rotate <, 45, 0>
  translate <4, 4, -2>
}

box { <-1, -1, -1>, <1, 1, 1>
  pigment { color rgb <0.3, 0.3, 0.8>}
  finish {ambient 0.2, diffuse 0.7}
  scale <1.5, .25, .25>
  rotate <0, 45, 0>
  translate <4, 1, -2>
}

box { <-1, -1, -1>, <1, 1, 1>
  pigment { color rgb <0.3, 0.3, 0.8>}
  finish {ambient 0.2, diffuse 0.7}
  scale <1.5, .25, .25>
  rotate <0, 45, 0>
  translate <4, -2, -2>
}

box { <-1, -1, -1>, <1, 1, 1>
  pigment { color rgb <0.3, 0.3, 0.8>}
  finish {ambient 0.2, diffuse 0.7}
  scale <.25, 3.3, .25>
  rotate <0, 45, 0>
  translate <5, 1, -3>
}

sphere { <4.75, 1, -3>, 3.25
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior 1.7 }
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <1.0, 0, .5>}
  finish {ambient .3, diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior .5}
  scale <3, 6, 3>
  translate <-3.5, -1.25, 3.75>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <1.0, 0, .5>}
  finish {ambient .3, diffuse 0.4 specular 0.6 roughness 0.0001 refraction 1.0 ior 1.02}
  scale <1, 10, 1>
  translate <0, -5, 0>
}

plane {<0, 1, 0>, -5
  pigment {color rgb <0.9, 0.9, 0.9>}
  finish {ambient 0.3 diffuse 0.8 reflection .75}
}
//My created scene for project 4

camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-100, 100, 80> color rgb <.75, .75, .75>}

light_source {<100, 100, 80> color rgb <.75, .75, .75>}

sphere { <0, 1, 0>, 2
  pigment { color rgbf <1.0, 1.0, 1.0, 0.95> }
  finish { ambient 0.02 diffuse 0.4 specular 10.0 roughness 0.001 refraction 1.0 ior 1.9}
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <1.0, 0, 0>}
  finish {ambient 0.3 diffuse 0.4 reflection 0.3}
  scale <1, 4, 1>
  translate<0, 0, -4>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <0, 1.0, 0>}
  finish {ambient 0.3 diffuse 0.4 reflection 0.3}
  scale <1, 4, 1>
  rotate <0, 0, 180>
  translate<-1, 4, -4>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <0, 0, 1.0>}
  finish {ambient 0.3 diffuse 0.4 reflection 0.3}
  scale <1, 4, 1>
  rotate <0, 0, 180>
  translate <1, 4, -4>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <1.0, 1.0, 0>}
  finish {ambient 0.3 diffuse 0.4 reflection 0.3}
  scale <1, 4, 1>
  translate <-2, 0, -4>
}

triangle {
  <1, 0, 0>
  <0, 1, 0>
  <-1, 0, 0>
  pigment {color rgb <0, 1.0, 1.0>}
  finish {ambient 0.3 diffuse 0.4 reflection 0.3}
  scale <1, 4, 1>
  translate <2, 0, -4>
}

plane {<0, 1, 0>, -4
  pigment {color rgb <0.2, 0.2, 0.8>}
  finish {ambient 0.3 diffuse 0.8}
}

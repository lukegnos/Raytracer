camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.5, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

box { <-2, -5, -5>, <2, 5, 5>
  pigment { color rgbf <1.0, 0, 0>}
  finish {ambient 0.2 diffuse 0.8}
  rotate <0, -60, 0>
  translate <3, 0, -5>
}

plane {<0, 1, 0>, -4
  pigment {color rgb <0.2, 0.2, 0.8>}
  finish {ambient 0.4 diffuse 0.8}
  translate <0, -2, 0>
}

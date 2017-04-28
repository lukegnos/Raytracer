//Scene 3 for final

camera {
  location  <0, 0, 20>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-20, 20, 20> color rgb <.3, 0, 0>}
light_source {<0, 20, 20> color rgb <0, 0.3, 0>}
light_source {<20, 20, 20> color rgb <0, 0, 0.3>}
light_source {<-40, 20, 20> color rgb <.3, 0, 0.3>}
light_source {<40, 20, 20> color rgb <0, .3, 0.3>}

light_source {<0, 20, -10> color rgb <.3, 0.3, 0>}
light_source {<-20, 20, -10> color rgb <0.15, 0, 0.3>}
light_source {<20, 20, -10> color rgb <0.3, 0.15, 0>}
light_source {<-40, 20, -10> color rgb <.15, .3, 0>}
light_source {<40, 20, -10> color rgb <0.3, 0.15, 0.3>}

sphere { <-4.5, 0, 0>, 2
  pigment { color rgbf <1.0, 1.0, 1.0>}
  finish { ambient 0.02 diffuse 0.4 specular 0.4 roughness 0.001}
}

sphere { <0, 0, 0>, 2
  pigment { color rgbf <1.0, 1.0, 1.0>}
  finish { ambient 0.02 diffuse 0.4 reflection .80 specular 0.4 roughness 0.001}
}

sphere { <4.5, 0, 0>, 2
  pigment { color rgbf <1.0, 1.0, 1.0>}
  finish { ambient 0.02 diffuse 0.4 specular 0.4 roughness 0.001}
}

//Floor
plane {<0, 1, 0>, -5
  pigment {color rgb <0.2, 0.2, 0.2>}
  finish {ambient 0.3 diffuse 1.75 reflection .8}
}
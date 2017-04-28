//Scene 4 for final

camera {
  location  <0, 0, 20>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source{<50, 100, 100> color rgb <1.0, 1.0, 1.0>}

sphere { <-4, 0, 0>, 2
  pigment { color rgbf <1.0, 0, 0>}
  finish { ambient 0.02 diffuse 0.4 reflection .80 specular 0.4 roughness 0.001}
}

sphere { <0, 0, 0>, 2
  pigment { color rgbf <0, 1.0, 0>}
  finish { ambient 0.02 diffuse 0.4 reflection .80 specular 0.4 roughness 0.001}
}

sphere { <4, 0, 0>, 2
  pigment { color rgbf <0, 0, 1.0>}
  finish { ambient 0.02 diffuse 0.4 reflection .80 specular 0.4 roughness 0.001}
}

plane { <0, 0, 1> -4
  pigment {color rgb <0.2, 0.2, 0.2>}
  finish {ambient 0.3 diffuse 1.5 reflection .8}
}

//Floor
plane {<0, 1, 0>, -5
  pigment {color rgb <0.2, 0.2, 0.2>}
  finish {ambient 0.3 diffuse 1.5 reflection .8}
}
//My created scene for project 5

camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<0, 10, 10> color rgb <1, 1, 1>}

sphere { <0, 0, 0>, 1.5
  pigment { color rgbf<0.3, .3, .3>}
  finish { ambient 0.2 diffuse 0.4 }
}

sphere { <-2.5, 2.5, 0>, 2
  pigment { color rgbf<1.0, 0, 0>}
  finish { ambient 0.2 diffuse 0.4 }
}

sphere { <-2.5, -2.5, 0>, 2
  pigment { color rgbf<0, 1.0, 0>}
  finish { ambient 0.2 diffuse 0.4 }
}

sphere { <2.5, 2.5, 0>, 2
  pigment { color rgbf<0, 0, 1.0>}
  finish { ambient 0.2 diffuse 0.4 }
}

sphere { <2.5, -2.5, 0>, 2
  pigment { color rgbf<1.0, 0.5, 0>}
  finish { ambient 0.2 diffuse 0.4 }
}

plane {<0, 1, 0>, -8
  pigment {color rgb <0.2, 0.2, 0.8>}
  finish {ambient 0.3 diffuse 0.8}
}
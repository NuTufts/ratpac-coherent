/*
//Collumator Geometry
{
name: "GEO",
index: "world",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "",
type: "box",
size: [500.,500.,500.],
position: [0.,0.,0.],
material: "air",
color: [0.,1.,0.,0.],
drawstyle: "solid"
}

{
name: "GEO",
index: "cuvette",
valid_begin: [0,0],
valid_end: [0,0],
mother: "world",
type: "sphere",
r_max: 20.5,
r_min: 18.5,
theta_start: 20.0, 
theta_delta: 160.0,
position: [0.,0.,0.],
material: "quartz",
color: [1.,1.,1.,0.3],
drawstyle: "solid"
}

{
name: "GEO", 
index: "tube3", 
valid_begin: [0,0], 
valid_end: [0,0],
mother:"world",
type: "tube", 
r_max: 10.0, 
r_min: 8.0, 
size_z: 15.0,
position: [0.,0.,35], 
material: "quartz",
color: [1.,1.,1.,0.3],
} 

{ 
name: "GEO", 
index: "tube1", 
valid_begin: [0,0],
valid_end: [0,0],
mother: "world",
type: "tube",
r_max: 7.9,
r_min: 4.0,
size_z: 15.0,
position: [0.,0.,35.0], 
material: "quartz", 
color: [1,1,1], 
}

{ 
name: "GEO", 
index: "tube2", 
valid_begin: [0,0], 
valid_edn: [0,0], 
mother: "world", 
type: "tube", 
r_max: 12.5, 
r_min: 4.0, 
size_z: 20.0, 
position: [0.,0.,70.0], 
material: "quartz", 
color: [1,1,1], 
} 

{
name: "GEO",
index: "scintillator",
valid_begin: [0,0],
valid_end: [0,0],
mother: "world", 
type: "sphere",
r_max: 18.5,    //approx
theta_start: 90.0, 
theta_delta: 90.0,
position: [0.,0.,0.],
//material: "mineral_oil",
//material: "water",
material: "scintillator", 
color: [0.,0.,1.,0.6],
}
*/

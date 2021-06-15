{
  name:"GEO",
  valid_begin: [0,0],
  valid_end: [0,0],
  gdml_file: "sipm_array.gdml",
  opdet_lv_name: ["volPanelSiPMdet"],
  opchannel_pv_name: ["pvPanelSiPMdet"],
}

{
  name:"GEO",
  index: "sipm_array_surface",
  valid_begin: [0,0],
  valid_end: [0,0],
  type: "skin",
  mother: "volPanelSiPMarray",
  logicalvolume: "volPanelSiPMarray",
  surface: "ptfe",
}

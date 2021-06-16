{
  name:"GEO",
  valid_begin: [0,0],
  valid_end: [0,0],
  gdml_file: "lar1ton_pmt_and_sipm.gdml",
  opdet_sdnames: ["pmtsd","sipm"]
}
{
  name:"SDCONFIG:pmtsd",
  valid_begin: [0,0],
  valid_end: [0,0],
  simplesdname: "pmtsd"
  lv_names: ["volTopPMTVac"],
  pv_name:  ["pvPMTtop","pvPMTbot"],
  qefile: "dat/qe_ham_11410-10.root",
  applyqe: true,
  copynodepth: 0,
  copynopvname: [],
  copynooffset: [],
  copynostride: [],
}
{
  name:"SDCONFIG:sipm",
  valid_begin: [0,0],
  valid_end: [0,0],
  simplesdname: "sipm"
  lv_names: ["volPanelSiPMdet"],
  pv_name:  ["pvPanelSiPMarray"]
  qefile: "dat/qe_ham_mppc_s14520.root",
  applyqe: true,
  copynodepth:  5,
  copynopvname:   ["volPanelSiPMarray","volPanel"]
  copynostrides:  [68,40],
  copynooffsets: 300,
}
{
  name:"GEO",
  index: "border_cryostat_wall1",
  valid_begin: [0,0],
  valid_end: [0,0],
  mother:"border",
  type:"border",
  volume1:"pvCryostat",
  volume2:"pvLArTube",
  surface:"stainless_steel",
  reverse:0,
}
{
  name:"GEO",
  index: "border_cryostat_wall2",
  valid_begin: [0,0],
  valid_end: [0,0],
  mother:"border",
  type:"border",
  volume1:"pvCryostat",
  volume2:"pvLArTube",  
  surface:"stainless_steel",
  reverse:0,
}
{
  name:"GEO",
  index: "surf_endcap",
  valid_begin: [0,0],
  valid_end: [0,0],
  mother:"skin",
  type:"skin",
  logicalvolume:"volEndcap",
  surface:"ptfe",
  reverse:0,
}
{
  name:"GEO",
  index: "surf_volPanel",
  valid_begin: [0,0],
  valid_end: [0,0],
  mother:"skin",
  type:"skin",
  logicalvolume:"volPanel",
  surface:"ptfe",
  reverse:0,  
}
{
  name:"GEO",
  index: "volPanelSiPMarray",
  valid_begin: [0,0],
  valid_end: [0,0],
  mother:"skin",
  type:"skin",
  logicalvolume:"volPanelSiPMarray",
  surface:"ptfe",
  reverse:0,  
}
{
  name:"GEO",
  index: "border_toppmt_and_lar1",
  valid_begin: [0,0],
  valid_end: [0,0],
  mother:"border",
  type:"border",
  volume1:"pvInnerTubePMT",
  volume2:"pvTopPMTGlass",
  surface:"glass",
  reverse:0,
}
{
  name:"GEO",
  index: "border_toppmt_and_lar2",
  valid_begin: [0,0],
  valid_end: [0,0],
  mother:"border",
  type:"border",
  volume1:"pvTopPMTGlass",  
  volume2:"pvInnerTubePMT",
  surface:"glass",
  reverse:0,
}
{
  name:"GEO",
  index: "border_botpmt_and_lar1",
  valid_begin: [0,0],
  valid_end: [0,0],
  mother:"border",
  type:"border",
  volume1:"pvInnerTubePMT",
  volume2:"pvBotPMTGlass",
  surface:"glass",
  reverse:0,
}
{
  name:"GEO",
  index: "border_botpmt_and_lar2",
  valid_begin: [0,0],
  valid_end: [0,0],
  mother:"border",
  type:"border",
  volume1:"pvBotPMTGlass",  
  volume2:"pvInnerTubePMT",
  surface:"glass",
  reverse:0,
}


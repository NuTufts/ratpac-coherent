
# COHERENT rat-pac fork

Simulation code for COHERENT LAr

## Features added in this Fork

* GDML integration
* Generic sensitive detector for optical photons. Goal is tool for quick prototyping.
* Marley generator integration (to do)
* Cevens generator integration (to do)
* CRY generator integration (to do)
* Simple waveform generation for generic sensitive detector (to do)

## Checking out

* using git flow pattern (more info: https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow)
* git clone https://github.com/NuDot/ratpac-nudot.git
* git checkout develop (this moves you to the develop branch)

## Dependencies
* ROOT 6 (last developed with 6.14/02)
* Geant4 (last developed with geant4.10.6.p03)
* CMake (last used version 3.10.2)
* Python 3 (or Python 2)
* Google Protobuf (on ubuntu install libprotobuf-dev)
* ZeroMQ (on ubuntu install package libzmq3-dev)


## building
* Now using CMake!
* make sure ROOT and geant4 environment variables set (did you run `thisroot.sh` and `geant4.sh`?)
* If first time, run: `./configure`. This will make `env.sh`.
* `source env.sh`. You need to do this each time you open a new shell.
* `mkdir build`. You only need to do this the first time (or if you delete the `build` folder).
* `cd build`
* `cmake -DCMAKE_BUILD_TYPE=Debug ../`
* `make`
* `make install`

## Running
* `rat <macro file 1> <macro file 2> ... -`
* The number of macros provided can be >= 0. See the `macro` folder for examples.
* Add the `-` if you want to end up in the interactive terminal.
* `rat --help` to print options.

## Adding a feature
* git checkout develop
* git checkout -b [username]_[featurename]
* do something awesome
* to put your code onto develop: git checkout develop; git merge [username]_[featurename]
* delete the branch

## Chroma Interface Optional

This copy has remnants of an extremely developmental feature: an interface to Chroma, which simulates photon transport on a GPU.
Current state is that it is not really working.

## Geometry

## Optical Detector

* Needed to add new type of Sensitive Detector class for optical detectors
* RAT optical detectors too tied to PMTs
* Created GLG4SimpleOpDetSD.  No fancy physics. If opticalphoton hits it, then a hit gets made. (later we can maybe configure this.)
* A way of assigning channelIDs through GDML and RAT configuration files.
  Allow way to assign IDs to repeated volumes.

### Steps to use this
* In your GDML, copy numbers should be given to the physical volumes that should be used to calculate the channel ID.
  For example, in the `lar1ton` geometry, we want to implement a large arrangement of SiPMs.
  The SiPMs are grouped at three levels.
  The first level is a 2x2 array. These arrays are then embedded into a panel.
  Several panels will be placed tegether in a ring and together will define the inner detector volume.
  In the GDML that implements the `lar1ton` geometry, copy numbers are provided to (1) the SiPM in the arrays,
  (2) the arrays on a panel, and (3) to each panel.

```
[ (1) the SiPMs in an array. In data/lar1ton/idwall_sipm_panel_array_placements.xml]
<volume name="volPanelSiPMarray">
  <materialref ref="ptfe"/>
  <solidref ref="PanelSiPMarraySolid"/>
    <physvol name="pvPanelSiPMdet0" copynumber="0">
      <volumeref ref="volPanelSiPMdet"/>
      <positionref ref="pos_sipm_panel_sd0"/>
    </physvol>
    <physvol name="pvPanelSiPMdet1" copynumber="1">
      <volumeref ref="volPanelSiPMdet"/>
      <positionref ref="pos_sipm_panel_sd1"/>
    </physvol>
    <physvol name="pvPanelSiPMdet2" copynumber="2">
      <volumeref ref="volPanelSiPMdet"/>
      <positionref ref="pos_sipm_panel_sd2"/>
    </physvol>
    <physvol name="pvPanelSiPMdet3" copynumber="3">
      <volumeref ref="volPanelSiPMdet"/>
      <positionref ref="pos_sipm_panel_sd3"/>
    </physvol>
</volume>

[ (2) arrays on a panel. In data/lar1ton/idwall_sipm_panel_array_placements.xml]
<volume name="volPanel">
  <materialref ref="ptfe"/>
  <solidref ref="IDpanel"/>
  <physvol name="pvPanelSiPMarray0" copynumber="0">
      <volumeref ref="volPanelSiPMarray"/>
      <positionref ref="pos_sipm_array_in_panel0"/>
    </physvol>
  <physvol name="pvPanelSiPMarray1" copynumber="1">
      <volumeref ref="volPanelSiPMarray"/>
      <positionref ref="pos_sipm_array_in_panel1"/>
    </physvol>
  <physvol name="pvPanelSiPMarray2" copynumber="2">
      <volumeref ref="volPanelSiPMarray"/>
      <positionref ref="pos_sipm_array_in_panel2"/>
    </physvol>
...

[(3) The placement of the panels, in data/lar1ton/idwall_sipm_panel_placements.xml]
<physvol name="pvIDPanel0" copynumber="0">
  <volumeref ref="volPanel"/>
  <rotationref ref="rot_panel_rotZ_0"/>
</physvol>
<physvol name="pvIDPanel1" copynumber="1">
  <volumeref ref="volPanel"/>
  <rotationref ref="rot_panel_rotZ_1"/>
</physvol>
<physvol name="pvIDPanel2" copynumber="2">
  <volumeref ref="volPanel"/>
  <rotationref ref="rot_panel_rotZ_2"/>
</physvol>
...

```
* In the GEO Table, one needs to configure the different instances of `GLG4SimpleOpDet`.
The first thing is to make a list of names for different instances.
This list should be assigned to the key, "opdet_sdnames".
```
[In `data/lar1ton/lar1ton..geo`]
{
name: "GEO",
valid_begin: [0, 0],
valid_end: [0, 0],
gdml_file: "lar1ton_pmt_and_sipm.gdml",
opdet_sdnames: ["sipm","pmtsd"]
}
```

* Next, one needs a configuration block, names `SDCONFIG:<name of Sensative detector>`.
{
  name:"SDCONFIG:sipm",
  valid_begin: [0,0],
  valid_end: [0,0],
  simplesdname: "sipm"
  lv_names: ["volPanelSiPMdet"],
  pv_names: ["pvPanelSiPMarray"]
  qefile: "dat/qe_ham_mppc_s14520.root",
  applyqe: true,
  copynopvnames:   ["pvIDPanel","pvPanelSiPMarray","pvPanelSiPMdet"]
  copynostrides:   [40,68,4],
  copynooffset: 300,
}
````

[in PMTINFO.ratdb file]
(right now a hack is required. Dummy PMTs need to be made, one for each optdet.)

In the above clip, two optical detectors are defined.
The number after "posSiPM" is the channel ID given to that optical detector.


# RAT (is an Analysis Tool), Public Edition
--------------------------------------------
RAT is a simulation and analysis package built with GEANT4, ROOT, and C++,
originally developed by S. Seibert for the Braidwood Collaboration. Versions
of RAT are now being used and developed by several particle physics
experiments.

RAT combines simulation and analysis into a single framework, which allows
analysis code to trivially access the same detector geometry and physics
parameters used in the detailed simulation.

RAT follows the "AMARA" principle: As Microphysical as Reasonably Achievable.
Each and every photon is tracked through an arbitrarily detailed detector
geometry, using standard GEANT4 or custom physics processes. PMTs are fully
modeled, and detected photons may be propagated to a simulation of front-end
electronics and DAQ.

This generic version is intended as a starting point for collaborations
looking for an easy-to-learn, extensible detector simulation and analysis
package that works out of the box. Once acquainted with RAT, it is easy to
customize the geometry elements, physics details, data structure, analysis
tools, etc., to suit your experiment's needs.


DPGA:
==============================
(Based on Arthur's Geant4 code)
***
Run Geant4 code: ( works with 4.10.3 )
----------------
1. Create folder in DPGA (for example, `build`).
2. Go to created folder _cd `build`_.
3. Type in terminal " _cmake ..._" .
4. Type in terminal _./DPGA [macros]_ for batch mode or _./DPGA_ for visualisation (OGL by defaul).
***
Run my data process code:
--------------------------
1. Root6 should be installed (my version is 6.12).
2. Copy `ProcessData.cxx` to DPGA/`build` and create folder `images` in DPGA/`build`.
3. ***IMPORTANT:*** input file for this code should call `data.root` (or change on line 88 TFile* dataFile = TFile::Open("data.root")).
4. Type _root_ in terminal.
5. Type _.x ProcessData.cxx_.
***
***
LYSO (with optical photons):
===========================
(Based on B3 example)
***
Run Geant4 code: ( works with 4.10.3 )
----------------
1. Create folder in lyso-crystal (for example, `build`).
2. Go to created folder _cd `build`_.
3. Type in terminal " _cmake ..._" .
4. Type in terminal _./lyso [macros]_ for batch mode or _./lyso_ for visualisation (OGL by defaul).
***
Run my data process code:
--------------------------
1. Root6 should be installed (my version is 6.12).
2. Copy `processData.cxx` to lyso-crystal/`build` and create folder `images` in lyso-crystal/`build`.
3. ***IMPORTANT:*** input file for this code should call `data.root` (or change on line 63 TFile* dataFile = TFile::Open("data.root")).
4. Type _root_ in terminal.
5. Type _.x ProcessData.cxx_.
***
More about optical photons in lyso-crystal/README.
***
***
Feel free to contact me: **vadymkedych@gmail.com** .

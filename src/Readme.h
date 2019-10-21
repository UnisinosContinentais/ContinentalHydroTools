/*

sink and destroy process

C:\Git\ContinentalHydroTools\build\release\console\ContinentalHydroToolsConsole.exe 1 "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "SinkDestroy"
"C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "SinkDestroy"
"C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "SinkDestroyFlowDirection" 100000 500000 2 0


FlowAccumulation
C:\Git\ContinentalHydroTools\build\release\console\ContinentalHydroToolsConsole.exe 2 "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "SinkDestroyFlowDirection" "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "FlowAccumulation"

streamDefinition
C:\Git\ContinentalHydroTools\build\release\console\ContinentalHydroToolsConsole.exe 3 "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "FlowAccumulation" "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "StreamDefinition" 1


streamSegmention
C:\Git\ContinentalHydroTools\build\release\console\ContinentalHydroToolsConsole.exe 4 "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "SinkDestroyFlowDirection" "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "StreamDefinition" "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "StreamSegmentation"

catchmentDelineation
C:\Git\ContinentalHydroTools\build\release\console\ContinentalHydroToolsConsole.exe 5 "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "SinkDestroyFlowDirection" "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "StreamSegmentation" "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "CatchmentDelineation"

watershedDelineation
C:\Git\ContinentalHydroTools\build\release\console\ContinentalHydroToolsConsole.exe 6 "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "SinkDestroyFlowDirection" "C:/Users/cristianheylmann/extracted5_streamprofile_hydrotools_0.h5" "WatershedDelineation" 15 15
*/

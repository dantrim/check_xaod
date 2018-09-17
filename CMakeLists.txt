cmake_minimum_required( VERSION 3.2 FATAL_ERROR )

#find_package( AnalysisBase 21.2 REQUIRED )

atlas_subdir( check_xaod )

atlas_depends_on_subdirs(
    PRIVATE
    Control/AthToolSupport/AsgTools
    Control/xAODRootAccess
    Event/xAOD/xAODEgamma
    Event/xAOD/xAODMuon
    Event/xAOD/xAODCore
    Event/xAOD/xAODJet
    PhysicsAnalysis/Interfaces/AsgAnalysisInterfaces
    PhysicsAnalysis/Interfaces/MuonAnalysisInterfaces
    PhysicsAnalysis/Interfaces/EgammaAnalysisInterfaces
)

find_package( ROOT REQUIRED COMPONENTS RIO Hist Tree Net Core )
set(_common
    INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
    LINK_LIBRARIES ${ROOT_LIBRARIES}
    xAODRootAccess
    xAODEgamma
    xAODMuon
    xAODJet
    AsgTools
)

atlas_add_executable( check_xaod util/check_xaod.cxx ${_common})

set(CURRENT_PDB "C:/Users/jorda/source/repos/JordanJRead/OpenGLRenderer/example-game/scripts/build/Release/Scripts.pdb")
set(RENAMED_PDB "C:/Users/jorda/source/repos/JordanJRead/OpenGLRenderer/example-game/scripts/build/Release/Scripts_old.pdb")

if(EXISTS "${CURRENT_PDB}")
    file(RENAME "${CURRENT_PDB}" "${RENAMED_PDB}")
endif()

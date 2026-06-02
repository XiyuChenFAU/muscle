param(
    [string]$EnvName = "muscle_path_build",
    [string]$BuildDir = "build-windows"
)

$ErrorActionPreference = "Stop"

if (-not (Get-Command conda -ErrorAction SilentlyContinue)) {
    throw "conda was not found. Install Miniforge or Miniconda first, then rerun this script from PowerShell."
}

conda create -y -n $EnvName -c conda-forge cmake ninja qt=5 casadi ipopt openblas

$prefix = conda run -n $EnvName powershell -NoProfile -Command '$env:CONDA_PREFIX'
$prefix = $prefix.Trim()

$qtDir = Join-Path $prefix "Library\lib\cmake\Qt5"
$casadiInclude = Join-Path $prefix "Library\include"
$casadiLib = Join-Path $prefix "Library\lib\casadi.lib"

conda run -n $EnvName cmake -S . -B $BuildDir -G Ninja `
    -DQt5_DIR="$qtDir" `
    -DCasADi_INCLUDE_DIRS="$casadiInclude" `
    -DCasADi_LIBRARIES="$casadiLib"

conda run -n $EnvName cmake --build $BuildDir

Write-Host ""
Write-Host "Build finished. The executable should be under $BuildDir if all dependencies were found."
Write-Host "If CMake cannot find a C++ compiler, install Visual Studio Build Tools with the Desktop C++ workload."

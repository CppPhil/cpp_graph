Push-Location .

$mingw_dir = 'mingw_dist'
$nuwen_archive = 'mingw-17.1-without-git.exe'
$url = "https://nuwen.net/files/mingw/${nuwen_archive}"

if (-Not (Test-Path -Path $mingw_dir)) {
  mkdir $mingw_dir
}

Set-Location $mingw_dir

if (Test-Path -Path 'MinGW') {
  Write-Output "${mingw_dir}/MinGW exists, assuming it contains a valid MinGW distribution!"
} else {
  bitsadmin.exe /transfer "MinGW Download" "${url}" "${pwd}\\${nuwen_archive}"
  
  if (-Not ($LASTEXITCODE -eq "0")) {
    Write-Output "Failed to download MinGW from ${url}"
    Pop-Location
    exit 1
  }
  
  & .\$nuwen_archive -y | Wait-Process
  
  if (-Not ($LASTEXITCODE -eq "0")) {
    Write-Output "Could not extract ${nuwen_archive}"
    Pop-Location
    exit 1
  }
}

Set-Location ..

$build_dir = 'build'

if (-Not (Test-Path -Path $build_dir)) {
  mkdir $build_dir
}

Set-Location $build_dir

Write-Output ""
Write-Output ""
Write-Output "~~~~~~~~~~~~ Starting debug build (MinGW) ~~~~~~~~~~~~"
Write-Output ""

if (-Not (Test-Path -Path 'Debug')) {
  mkdir Debug
}

Set-Location Debug
cmd.exe /c "..\..\$mingw_dir\MinGW\set_distro_paths.bat && cmake -G ""MinGW Makefiles"" -DCMAKE_BUILD_TYPE=Debug ..\.."

cmd.exe /c "..\..\$mingw_dir\MinGW\set_distro_paths.bat && cmake --build . --config Debug"

if (-Not ($LASTEXITCODE -eq "0")) {
  Write-Output "cmake --build for Debug mode failed!"
  Pop-Location
  exit 1
}

Write-Output ""
Write-Output "~~~~~~~~~~~~ Completed debug build (MinGW) ~~~~~~~~~~~~"
Write-Output ""
Write-Output ""

Pop-Location
Push-Location .
Set-Location $build_dir

Write-Output ""
Write-Output ""
Write-Output "~~~~~~~~~~~~ Starting release build (MinGW) ~~~~~~~~~~~~"
Write-Output ""

if (-Not (Test-Path -Path 'Release')) {
  mkdir Release
}

Set-Location Release
cmd.exe /c "..\..\$mingw_dir\MinGW\set_distro_paths.bat && cmake -G ""MinGW Makefiles"" -DCMAKE_BUILD_TYPE=Release ..\.."

cmd.exe /c "..\..\$mingw_dir\MinGW\set_distro_paths.bat && cmake --build . --config Release"

if (-Not ($LASTEXITCODE -eq "0")) {
  Write-Output "cmake --build for Release mode failed!"
  Pop-Location
  exit 1
}

Write-Output ""
Write-Output "~~~~~~~~~~~~ Completed release build (MinGW) ~~~~~~~~~~~~"
Write-Output ""
Write-Output ""

Pop-Location
exit 0

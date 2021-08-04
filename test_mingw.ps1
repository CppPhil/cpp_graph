Push-Location .

$build_dir = 'build'

if (-Not (Test-Path -Path $build_dir)) {
  mkdir $build_dir
}

& .\build_mingw.ps1

if (-Not ($LASTEXITCODE -eq "0")) {
  Write-Output "Running build.ps1 failed!"
  Pop-Location
  exit 1
}

Set-Location $build_dir

Write-Output ""
Write-Output ""
Write-Output ""
Write-Output ">>>>>>>>>>>>>>>>>>>>>>> Starting debug test run (MinGW)"

Set-Location Debug

& .\graph_app.exe

if (-Not ($LASTEXITCODE -eq "0")) {
  Write-Output "Debug test run failed!"
  Pop-Location
  exit 1
}

Write-Output ">>>>>>>>>>>>>>>>>>>>>>> Completed debug test run (MinGW)"
Write-Output ""
Write-Output ""
Write-Output ""

Pop-Location
Push-Location .
Set-Location $build_dir

Write-Output ""
Write-Output ""
Write-Output ""
Write-Output ">>>>>>>>>>>>>>>>>>>>>>> Starting release test run (MinGW)"

Set-Location Release

& .\graph_app.exe

if (-Not ($LASTEXITCODE -eq "0")) {
  Write-Output "Release test run failed!"
  Pop-Location
  exit 1
}

Write-Output ">>>>>>>>>>>>>>>>>>>>>>> Completed release test run (MinGW)"
Write-Output ""
Write-Output ""
Write-Output ""

Pop-Location
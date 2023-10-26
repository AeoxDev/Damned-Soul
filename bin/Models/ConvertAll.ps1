Write-Output $Path

Remove-Item -Path .\Mdl -Recurse
New-Item -Force -Path .\Mdl -ItemType Directory
Remove-Item -Path .\Ani -Recurse
New-Item -Force -Path .\Ani -ItemType Directory

foreach ($File in Get-ChildItem "RawFBX\") {
	Write-Output $File.name
	./"AssimpOfflineConverter.exe" $File.Basename
}
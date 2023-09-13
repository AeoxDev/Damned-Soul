Write-Output $Path

New-Item -Force -Path .\Mdl -ItemType Directory

foreach ($File in Get-ChildItem "RawFBX\") {
	Write-Output $File.name
	./"AssimpOfflineConverter.exe" $File.Basename
}
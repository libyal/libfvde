# Script that synchronizes Windows versions of flex and bison.
#
# Version: 20230104

Function ExtractZip($Filename, $Destination)
{
	# AppVeyor does not seem to support extraction using "native ZIP" so we use 7z instead.
	$SevenZip = "C:\Program Files\7-Zip\7z.exe"

	If (-Not (Test-Path ${Destination}))
	{
		New-Item -ItemType directory -Path ${Destination} -Force | Out-Null
	}
	If (Test-Path ${SevenZip})
	{
		# PowerShell will raise NativeCommandError if 7z writes to stdout or stderr
		# therefore 2>&1 is added and the output is stored in a variable.
		# The leading & and single quotes are necessary to compensate for the spaces in the path.
		$Output = Invoke-Expression -Command "& '${SevenZip}' -y -o${Destination} x ${Filename} 2>&1"
	}
	else
	{
		$Shell = New-Object -ComObject Shell.Application
		$Archive = ${Shell}.NameSpace(${Filename})
		$Directory = ${Shell}.Namespace(${Destination})

		ForEach($FileEntry in ${Archive}.items())
		{
			${Directory}.CopyHere(${FileEntry})
		}
	}
}

$Version = "2.5.25"
$Filename = "${pwd}\win_flex_bison-${Version}.zip"
$Url = "https://github.com/lexxmark/winflexbison/releases/download/v${Version}/win_flex_bison-${Version}.zip"
$ExtractedPath = "win_flex_bison-${Version}"
$DestinationPath = "..\win_flex_bison"

If (Test-Path ${Filename})
{
	Remove-Item -Path ${Filename} -Force
}
Invoke-WebRequest -Uri ${Url} -OutFile ${Filename}

If (Test-Path ${ExtractedPath})
{
	Remove-Item -Path ${ExtractedPath} -Force -Recurse
}
ExtractZip -Filename ${Filename} -Destination "${pwd}\${ExtractedPath}"

Remove-Item -Path ${Filename} -Force

If (Test-Path ${DestinationPath})
{
	Remove-Item -Path ${DestinationPath} -Force -Recurse
}
Move-Item ${ExtractedPath} ${DestinationPath}


# Script that synchronizes Windows versions of flex and bison.
#
# Version: 20180405

Function DownloadFile($Url, $Destination)
{
	[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.SecurityProtocolType]::Tls12

	$Client = New-Object Net.WebClient
	${Client}.DownloadFile(${Url}, ${Destination})
}

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

$Version = "2.5.14"
$Filename = "${pwd}\winflexbison-${Version}.zip"
$Url = "https://github.com/lexxmark/winflexbison/archive/v${Version}.zip"
$ExtractedPath = "winflexbison-${Version}"
$DestinationPath = "..\win_flex_bison"

If (Test-Path ${Filename})
{
	Remove-Item -Path ${Filename} -Force
}
DownloadFile -Url ${Url} -Destination ${Filename}

If (Test-Path ${ExtractedPath})
{
	Remove-Item -Path ${ExtractedPath} -Force -Recurse
}
ExtractZip -Filename ${Filename} -Destination "${pwd}"

Remove-Item -Path ${Filename} -Force

If (Test-Path ${DestinationPath})
{
	Remove-Item -Path ${DestinationPath} -Force -Recurse
}
Move-Item "${ExtractedPath}\bin\Release" ${DestinationPath}

Remove-Item -Path ${ExtractedPath} -Force -Recurse


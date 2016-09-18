# Script that synchronizes Windows versions of flex and bison.
#
# Version: 20160918

function DownloadFile($Url, $Destination)
{
	$Client = New-Object Net.WebClient
	${Client}.DownloadFile(${Url}, ${Destination})
}

function ExtractZip($Filename, $Destination)
{
	# AppVeyor does not seem to support extraction using "native ZIP" so we use 7z instead.
	$SevenZip = "C:\Program Files\7-Zip\7z.exe"

	if (-Not (Test-Path ${Destination}))
	{
		New-Item -ItemType directory -Path ${Destination} -Force | Out-Null
	}
	if (Test-Path ${SevenZip})
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

		foreach($FileEntry in ${Archive}.items())
		{
			${Directory}.CopyHere(${FileEntry})
		}
	}
}

$Filename = "${pwd}\win_flex_bison-2.5.6.zip"
$Url = "http://downloads.sourceforge.net/project/winflexbison/win_flex_bison-2.5.6.zip"
$ExtractedPath = "win_flex_bison-2.5.6"
$DestinationPath = "..\win_flex_bison"

if (Test-Path ${Filename})
{
	Remove-Item -Path ${Filename} -Force
}
DownloadFile -Url ${Url} -Destination ${Filename}

if (Test-Path ${ExtractedPath})
{
	Remove-Item -Path ${ExtractedPath} -Force -Recurse
}
ExtractZip -Filename ${Filename} -Destination "${pwd}\${ExtractedPath}"

if (Test-Path ${DestinationPath})
{
	Remove-Item -Path ${DestinationPath} -Force -Recurse
}
Move-Item ${ExtractedPath} ${DestinationPath}


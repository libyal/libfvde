# Info tool testing script
#
# Version: 20200223

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$Profiles = @("fvdeinfo")
$OptionsPerProfile = @("")
$OptionSets="offset password recovery_password";

$InputGlob = "*"

Function GetTestExecutablesDirectory
{
	$TestExecutablesDirectory = ""

	ForEach (${VSDirectory} in "msvscpp vs2008 vs2010 vs2012 vs2013 vs2015 vs2017 vs2019" -split " ")
	{
		ForEach (${VSConfiguration} in "Release VSDebug" -split " ")
		{
			ForEach (${VSPlatform} in "Win32 x64" -split " ")
			{
				$TestExecutablesDirectory = "..\${VSDirectory}\${VSConfiguration}\${VSPlatform}"

				If (Test-Path ${TestExecutablesDirectory})
				{
					Return ${TestExecutablesDirectory}
				}
			}
			$TestExecutablesDirectory = "..\${VSDirectory}\${VSConfiguration}"

			If (Test-Path ${TestExecutablesDirectory})
			{
				Return ${TestExecutablesDirectory}
			}
		}
	}
	Return ${TestExecutablesDirectory}
}

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Host "Missing test executables directory." -foreground Red

	Exit ${ExitFailure}
}

$TestExecutable = "${TestExecutablesDirectory}\fvdeinfo.exe"

If (-Not (Test-Path -Path "input"))
{
	Exit ${ExitSuccess}
}

For ($ProfileIndex = 0; $ProfileIndex -le ($Profiles.length - 1); $ProfileIndex += 1)
{
	$TestProfile = $Profiles[$ProfileIndex]
	$TestProfileDirectory = "input\.${TestProfile}"
	$Options = $OptionsPerProfile[$ProfileIndex]

	Get-ChildItem -Path "input\*" | Foreach-Object
	{
		$TestSetDirectory = $_

		# TODO: check ignore file

		If (-Not (Test-Path -Path ${TestSetDirectory} -PathType Container))
		{
			Continue
		}
		Get-ChildItem -Path "${TestSetDirectory}\${InputGlob}" | Foreach-Object
		{
			$InputFile = $_

			# TODO: handle test data options

			Invoke-Expression ${TestExecutable} ${Options} ${InputFile}

			If (${LastExitCode} -ne ${ExitSuccess})
			{
				Break
			}
		}
		If (${LastExitCode} -ne ${ExitSuccess})
		{
			Break
		}
	}
}

Exit ${LastExitCode}


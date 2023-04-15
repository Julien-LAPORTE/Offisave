@echo off
chcp 1252 >nul
setlocal enabledelayedexpansion

set logs=C:\Users\Sam\Documents\test.log

goto commentaire
if exist !cheminSauvegarde!\Local\Thunderbird (
echo Installation de Thunderbird >>!logs! 2>&1
echo Installation de Thunderbird
if exist C:\WPI (
echo Installation via WPI >>!logs! 2>&1
C:\WPI\Install\Internet\thunderbird\ThunderbirdSetupx64.exe /s
) ELSE (
		if exist C:\pharmagest\PCInstall (
		echo Installation via PCInstall >>!logs! 2>&1
		C:\pharmagest\PCInstall\apps\Messagerie\thunderbird\ThunderbirdSetupFRx64.exe /s
			) ELSE (
				echo ^:^( Pas de WPI ou de PCInstall d'installé >>!logs! 2>&1
			)
		)
)
:commentaire


if exist C:\Windows (
echo Installation de Thunderbird >!logs! 2>&1
echo Installation de Thunderbird
if exist C:\WPI (
echo Installation via WPI >>!logs! 2>&1
C:\WPI\Install\Internet\thunderbird\ThunderbirdSetupx64.exe /s
) ELSE (
		if exist C:\pharmagest\PCInstall (
		echo Installation via PCInstall >>!logs! 2>&1
		C:\pharmagest\PCInstall\apps\Messagerie\thunderbird\ThunderbirdSetupFRx64.exe /s
			) ELSE (
				echo ^:^( Pas de WPI ou de PCInstall d'installé >>!logs! 2>&1
			)
		)
)
pause
; Copyright 2006 Eric Niebler
; Distributed under the Boost Software License, Version 1.0. (See
; accompanying file LICENSE_1_0.txt or copy at
; http://www.boost.org/LICENSE_1_0.txt)

!define VERSION 1.33.1
!define NORMALIZED_VERSION 1_33_1
!define PUBLISHER Boost.org
!define BOOST_ROOT C:\boost\boost_1_33_1
!define NAME "Boost C++ Libraries ${VERSION}"

;--------------------------------

; The file to write
OutFile "boost_${NORMALIZED_VERSION}_setup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\boost_${NORMALIZED_VERSION}

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\${PUBLISHER}\${VERSION}" "InstallRoot"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Boost (required)"

  SectionIn RO ; this is a Read-only section -- the user can't edit any of this
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  ; Put file there
  File /r "${BOOST_ROOT}\*"

  WriteRegStr HKLM "SOFTWARE\${PUBLISHER}\${VERSION}" "InstallRoot" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\boost_${NORMALIZED_VERSION}" "DisplayName" "${NAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\boost_${NORMALIZED_VERSION}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\boost_${NORMALIZED_VERSION}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\boost_${NORMALIZED_VERSION}" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd ; end the section

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\${NAME}"
  CreateShortCut "$SMPROGRAMS\${NAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\${NAME}\${NAME} Command Prompt.lnk" "%COMSPEC%" "/K $\"$INSTDIR\boostvars.bat$\""
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\boost_${NORMALIZED_VERSION}"
  ; Leave SOFTWARE\${PUBLISHER} in case there is another Boost version installed
  DeleteRegKey HKLM "SOFTWARE\${PUBLISHER}\${VERSION}"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\${NAME}\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\${NAME}"
  RMDir /r "$INSTDIR"

SectionEnd

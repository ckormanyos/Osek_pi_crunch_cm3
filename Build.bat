
@set build_command=.\make.exe -f ./Makefile %2 PI_SPIGOT_DIGITS=%1

@echo Executing Command: %build_command%

@%build_command%





// CONTROL CHECKING -------------------------------------	
sceCtrlPeekBufferPositive( 0, &pad, 1 );
dPadTimer	= max( 0, (dPadTimer -1) );
if ( pad.buttons )
	{
	// DPAD - UP
	if ( pad.buttons & SCE_CTRL_UP )
		{
		if ( previewActive == 0 )
			{
			if ( dPadTimer == 0 )
				{
				dPadTimer = 9;
				itemPressed--;
				if ( itemPressed < 0 ) { itemPressed = 0; }
				}
			}
		}
	// DPAD - DOWN
	if ( pad.buttons & SCE_CTRL_DOWN )
		{
		if ( previewActive == 0 )
			{
			if ( dPadTimer == 0 )
				{
				dPadTimer = 9;
				itemPressed++;
				switch ( screen )
					{
					case 0:		if ( itemPressed > itemCount_new 	) { itemPressed = itemCount_new; 	} break;
					case 1:		if ( itemPressed > itemCount_apps 	) { itemPressed = itemCount_apps; 	} break;
					case 2:		if ( itemPressed > itemCount_games 	) { itemPressed = itemCount_games; 	} break;
					case 3:		if ( itemPressed > itemCount_emu 	) { itemPressed = itemCount_emu; 	} break;
					case 4:		if ( itemPressed > itemCount_util 	) { itemPressed = itemCount_util; 	} break;
					case 5:		if ( itemPressed > itemCount_themes ) { itemPressed = itemCount_themes; } break;
					case 6:		if ( itemPressed > itemCount_demos 	) { itemPressed = itemCount_demos; 	} break;
					}
				if ( itemPressed < 0 ) { itemPressed = 0; }
				// MOVE ITEM ONTO SCREEN IF ABOVE OR BELOW SCREEN
				//while ( (originY +115 +(itemPanelHeight *itemPressed))
				
				}
			}
		}
	// DPAD - LEFT
	if ( pad.buttons & SCE_CTRL_LEFT )
		{
		
		}
	// DPAD - RIGHT
	if ( pad.buttons & SCE_CTRL_RIGHT )
		{
		
		}
	// BUTTON - CROSS
	if ( pad.buttons & SCE_CTRL_CROSS )
		{
		if ( previewActive == 0 )
			{
			if ( itemPressed >= 0 && clickable == 1 )
				{
				int previewReady	= 1;
				switch ( screen )
					{
					case 0:			// NEW
									if ( itemPressed < 0 || itemPressed > itemCount_new ) { previewReady = 0; break; }
									previewCategory		= catListNew[itemPressed].cat;
									strcpy( previewName, 		catListNew[itemPressed].name 		);
									strcpy( previewAuthor, 		catListNew[itemPressed].author 		);
									strcpy( previewVersion, 	catListNew[itemPressed].version 	);
									strcpy( previewDlSize, 		catListNew[itemPressed].dlSize 		);
									strcpy( previewRelease, 	catListNew[itemPressed].release 	);
									strcpy( previewDescription, catListNew[itemPressed].description );
									strcpy( previewLink, 		catListNew[itemPressed].link 		);
									strcpy( previewDir, 		catListNew[itemPressed].dir 		);
									previewEbootSize		= catListNew[itemPressed].ebootSize;
									break;
					case 1:			// APPS
									if ( itemPressed < 0 || itemPressed > itemCount_apps ) { previewReady = 0; break; }
									previewCategory		= catListApps[itemPressed].cat;
									strcpy( previewName, 		catListApps[itemPressed].name 			);
									strcpy( previewAuthor, 		catListApps[itemPressed].author 		);
									strcpy( previewVersion, 	catListApps[itemPressed].version 		);
									strcpy( previewDlSize, 		catListApps[itemPressed].dlSize 		);
									strcpy( previewRelease, 	catListApps[itemPressed].release 		);
									strcpy( previewDescription, catListApps[itemPressed].description 	);
									strcpy( previewLink, 		catListApps[itemPressed].link 			);
									strcpy( previewDir, 		catListApps[itemPressed].dir 			);
									previewEbootSize		= catListApps[itemPressed].ebootSize;
									break;
					case 2:			// GAMES
									if ( itemPressed < 0 || itemPressed > itemCount_games ) { previewReady = 0; break; }
									previewCategory		= catListGames[itemPressed].cat;
									strcpy( previewName, 		catListGames[itemPressed].name 			);
									strcpy( previewAuthor, 		catListGames[itemPressed].author 		);
									strcpy( previewVersion, 	catListGames[itemPressed].version 		);
									strcpy( previewDlSize, 		catListGames[itemPressed].dlSize 		);
									strcpy( previewRelease, 	catListGames[itemPressed].release 		);
									strcpy( previewDescription, catListGames[itemPressed].description 	);
									strcpy( previewLink, 		catListGames[itemPressed].link 			);
									strcpy( previewDir, 		catListGames[itemPressed].dir 			);
									previewEbootSize		= catListGames[itemPressed].ebootSize;
									break;
					case 3:			// EMULATORS
									if ( itemPressed < 0 || itemPressed > itemCount_emu ) { previewReady = 0; break; }
									previewCategory		= catListEmulators[itemPressed].cat;
									strcpy( previewName, 		catListEmulators[itemPressed].name 			);
									strcpy( previewAuthor, 		catListEmulators[itemPressed].author 		);
									strcpy( previewVersion, 	catListEmulators[itemPressed].version 		);
									strcpy( previewDlSize, 		catListEmulators[itemPressed].dlSize 		);
									strcpy( previewRelease, 	catListEmulators[itemPressed].release 		);
									strcpy( previewDescription, catListEmulators[itemPressed].description 	);
									strcpy( previewLink, 		catListEmulators[itemPressed].link 			);
									strcpy( previewDir, 		catListEmulators[itemPressed].dir 			);
									previewEbootSize		= catListEmulators[itemPressed].ebootSize;
									break;
					case 4:			// UTILITIES
									if ( itemPressed < 0 || itemPressed > itemCount_util ) { previewReady = 0; break; }
									previewCategory		= catListUtilities[itemPressed].cat;
									strcpy( previewName, 		catListUtilities[itemPressed].name 			);
									strcpy( previewAuthor, 		catListUtilities[itemPressed].author 		);
									strcpy( previewVersion, 	catListUtilities[itemPressed].version 		);
									strcpy( previewDlSize, 		catListUtilities[itemPressed].dlSize 		);
									strcpy( previewRelease, 	catListUtilities[itemPressed].release 		);
									strcpy( previewDescription, catListUtilities[itemPressed].description 	);
									strcpy( previewLink, 		catListUtilities[itemPressed].link 			);
									strcpy( previewDir, 		catListUtilities[itemPressed].dir 			);
									previewEbootSize		= catListUtilities[itemPressed].ebootSize;
									break;
					case 5:			// THEMES
									if ( itemPressed < 0 || itemPressed > itemCount_themes ) { previewReady = 0; break; }
									previewCategory		= catListThemes[itemPressed].cat;
									strcpy( previewName, 		catListThemes[itemPressed].name 		);
									strcpy( previewAuthor, 		catListThemes[itemPressed].author 		);
									strcpy( previewVersion, 	catListThemes[itemPressed].version 		);
									strcpy( previewDlSize, 		catListThemes[itemPressed].dlSize 		);
									strcpy( previewRelease, 	catListThemes[itemPressed].release 		);
									strcpy( previewDescription, catListThemes[itemPressed].description 	);
									strcpy( previewLink, 		catListThemes[itemPressed].link 		);
									strcpy( previewDir, 		catListThemes[itemPressed].dir 			);
									previewEbootSize		= catListThemes[itemPressed].ebootSize;
									break;
					case 6:			// DEMOS
									if ( itemPressed < 0 || itemPressed > itemCount_demos ) { previewReady = 0; break; }
									previewCategory		= catListDemos[itemPressed].cat;
									strcpy( previewName, 		catListDemos[itemPressed].name 			);
									strcpy( previewAuthor, 		catListDemos[itemPressed].author 		);
									strcpy( previewVersion, 	catListDemos[itemPressed].version 		);
									strcpy( previewDlSize, 		catListDemos[itemPressed].dlSize 		);
									strcpy( previewRelease, 	catListDemos[itemPressed].release 		);
									strcpy( previewDescription, catListDemos[itemPressed].description 	);
									strcpy( previewLink, 		catListDemos[itemPressed].link 			);
									strcpy( previewDir, 		catListDemos[itemPressed].dir 			);
									previewEbootSize		= catListDemos[itemPressed].ebootSize;
									break;
					}
				if ( previewReady == 1 )
					{
					strtok_r( previewDescription, "|", &previewDesLine2 );
					strtok_r( previewDesLine2,    "|", &previewDesLine3 );
					strtok_r( previewDesLine3, 	  "|", &previewDesLine4 );
					strtok_r( previewDesLine4, 	  "|", &previewDesLine5 );
					previewActive		=  1;
					previewListNumber	= itemPressed;
					// CHECK IF INSTALLED
					if ( access( previewDir, F_OK ) == -1 ) { preview_isInstalled = 0; }
					else  									{ preview_isInstalled = 1; }
					// CHECK IF DOWNLOADED
					if ( access( string_join( 3, VHBB_APP_ADDRESS_STORAGE_FILES, previewName, ".vpk" ), F_OK ) == -1 ) { preview_isDownloaded = 0; }
					else 																							   { preview_isDownloaded = 1; }
					// CHECK IF LATEST VERSION
					//isCurrent
					}
				}
			}
		}
	// BUTTON - CIRCLE
	if ( pad.buttons & SCE_CTRL_CIRCLE )
		{
		if ( settingsOpen == 0 ) { if ( previewActive == 1 )	{ previewActive = 0; itemPressed = previewListNumber; } }
		else					 { settingsOpen == 0; }
		}
	// BUTTON - SQUARE
	if ( pad.buttons & SCE_CTRL_SQUARE )
		{
		
		}
	// BUTTON - TRIANGLE
	if ( pad.buttons & SCE_CTRL_TRIANGLE )
		{
		if ( dPadTimer == 0 )
			{
			dPadTimer = 9;
			if ( settingsOpen != 1 )
				{
				settingsOpen	= 1;
				}
			else
				{
				settingsOpen	= 0;
				}
			}
		
		
		}
	// BUTTON - SELECT
	if ( pad.buttons & SCE_CTRL_SELECT )
		{
		break;
		}
	// BUTTON - START
	if ( pad.buttons & SCE_CTRL_START )
		{
		
		}
	// BUTTON - LEFT TRIGGER
	if ( pad.buttons & SCE_CTRL_LTRIGGER )
		{
		if ( previewActive == 0 )
			{
			if ( dPadTimer == 0 )
				{
				dPadTimer = 9;
				switch ( screen )
					{
					case 1:		screen = 0; originY = 0; break;
					case 2:		screen = 1; originY = 0; break;
					case 3:		screen = 2; originY = 0; break;
					case 4:		screen = 3; originY = 0; break;
					case 5:		screen = 4; originY = 0; break;
					case 6:		screen = 5; originY = 0; break;
					}
				}
			}
		}
	// BUTTON - RIGHT TRIGGER
	if ( pad.buttons & SCE_CTRL_RTRIGGER )
		{
		if ( previewActive == 0 )
			{
			if ( dPadTimer == 0 )
				{
				dPadTimer = 9;
				switch ( screen )
					{
					case 0:		screen = 1; originY = 0; break;
					case 1:		screen = 2; originY = 0; break;
					case 2:		screen = 3; originY = 0; break;
					case 3:		screen = 4; originY = 0; break;
					case 4:		screen = 5; originY = 0; break;
					case 5:		screen = 6; originY = 0; break;
					}
				}
			}
		}
	}
//-------------------------------------------------------
//-------------------------------------------------------
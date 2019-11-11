
// MusicPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "MusicPlayerDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "SupportedFormatDlg.h"
#include "AboutDlg.h"
#include "CTest.h"
#include "CListenTimeStatisticsDlg.h"
#include "CFloatPlaylistDlg.h"
#include "Playlist.h"
#include "InputDlg.h"
#include "FileRelateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMusicPlayerDlg 对话框

const UINT WM_TASKBARCREATED{ ::RegisterWindowMessage(_T("TaskbarCreated")) };	//注册任务栏建立的消息

CMusicPlayerDlg::CMusicPlayerDlg(wstring cmdLine, CWnd* pParent /*=NULL*/)
    : m_cmdLine{cmdLine}, CMainDialogBase(IDD_MUSICPLAYER2_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMusicPlayerDlg::~CMusicPlayerDlg()
{
    CCommon::DeleteModelessDialog(m_pLyricEdit);
    CCommon::DeleteModelessDialog(m_pLyricBatchDownDlg);
    //CCommon::DeleteModelessDialog(m_pSetPathDlg);
    CCommon::DeleteModelessDialog(m_pSoundEffecDlg);
    CCommon::DeleteModelessDialog(m_pFormatConvertDlg);
    CCommon::DeleteModelessDialog(m_pFloatPlaylistDlg);
}

bool CMusicPlayerDlg::IsTaskbarListEnable() const
{
#ifdef COMPILE_IN_WIN_XP
	return false;
#else
	return CWinVersionHelper::IsWindows7OrLater() && m_pTaskbar != nullptr;
#endif
}

void CMusicPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
    CMainDialogBase::DoDataExchange(pDX);
    //DDX_Control(pDX, IDC_TRACK_EDIT, m_track_edit);
    DDX_Control(pDX, IDC_PLAYLIST_LIST, m_playlist_list);
    DDX_Control(pDX, IDC_PATH_STATIC, m_path_static);
    DDX_Control(pDX, IDC_PATH_EDIT, m_path_edit);
    DDX_Control(pDX, ID_SET_PATH, m_set_path_button);
    DDX_Control(pDX, IDC_SEARCH_EDIT, m_search_edit);
    DDX_Control(pDX, IDC_CLEAR_SEARCH_BUTTON, m_clear_search_button);
}

BEGIN_MESSAGE_MAP(CMusicPlayerDlg, CMainDialogBase)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_COMMAND(ID_PLAY_PAUSE, &CMusicPlayerDlg::OnPlayPause)
    ON_COMMAND(ID_STOP, &CMusicPlayerDlg::OnStop)
    ON_COMMAND(ID_PREVIOUS, &CMusicPlayerDlg::OnPrevious)
    ON_COMMAND(ID_NEXT, &CMusicPlayerDlg::OnNext)
    ON_COMMAND(ID_REW, &CMusicPlayerDlg::OnRew)
    ON_COMMAND(ID_FF, &CMusicPlayerDlg::OnFF)
    ON_COMMAND(ID_SET_PATH, &CMusicPlayerDlg::OnSetPath)
    ON_COMMAND(ID_FIND, &CMusicPlayerDlg::OnFind)
    ON_COMMAND(ID_EXPLORE_PATH, &CMusicPlayerDlg::OnExplorePath)
    ON_WM_DESTROY()
    ON_COMMAND(ID_APP_ABOUT, &CMusicPlayerDlg::OnAppAbout)
    ON_COMMAND(ID_FILE_OPEN, &CMusicPlayerDlg::OnFileOpen)
    ON_COMMAND(ID_FILE_OPEN_FOLDER, &CMusicPlayerDlg::OnFileOpenFolder)
    ON_WM_DROPFILES()
    ON_WM_INITMENU()
    ON_COMMAND(ID_PLAY_ORDER, &CMusicPlayerDlg::OnPlayOrder)
    ON_COMMAND(ID_PLAY_SHUFFLE, &CMusicPlayerDlg::OnPlayShuffle)
    ON_COMMAND(ID_LOOP_PLAYLIST, &CMusicPlayerDlg::OnLoopPlaylist)
    ON_COMMAND(ID_LOOP_TRACK, &CMusicPlayerDlg::OnLoopTrack)
    ON_WM_MOUSEWHEEL()
    ON_WM_GETMINMAXINFO()
    ON_NOTIFY(NM_DBLCLK, IDC_PLAYLIST_LIST, &CMusicPlayerDlg::OnNMDblclkPlaylistList)
    ON_COMMAND(ID_OPTION_SETTINGS, &CMusicPlayerDlg::OnOptionSettings)
    ON_COMMAND(ID_RELOAD_PLAYLIST, &CMusicPlayerDlg::OnReloadPlaylist)
    ON_NOTIFY(NM_RCLICK, IDC_PLAYLIST_LIST, &CMusicPlayerDlg::OnNMRClickPlaylistList)
    ON_COMMAND(ID_PLAY_ITEM, &CMusicPlayerDlg::OnPlayItem)
    ON_COMMAND(ID_ITEM_PROPERTY, &CMusicPlayerDlg::OnItemProperty)
    ON_COMMAND(ID_EXPLORE_TRACK, &CMusicPlayerDlg::OnExploreTrack)
    ON_WM_HOTKEY()
    ON_COMMAND(ID_RE_INI_BASS, &CMusicPlayerDlg::OnReIniBass)
    ON_COMMAND(ID_SORT_BY_FILE, &CMusicPlayerDlg::OnSortByFile)
    ON_COMMAND(ID_SORT_BY_TITLE, &CMusicPlayerDlg::OnSortByTitle)
    ON_COMMAND(ID_SORT_BY_ARTIST, &CMusicPlayerDlg::OnSortByArtist)
    ON_COMMAND(ID_SORT_BY_ALBUM, &CMusicPlayerDlg::OnSortByAlbum)
    ON_COMMAND(ID_SORT_BY_TRACK, &CMusicPlayerDlg::OnSortByTrack)
    ON_COMMAND(ID_DELETE_FROM_DISK, &CMusicPlayerDlg::OnDeleteFromDisk)
    ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, &CMusicPlayerDlg::OnTaskbarcreated)
    ON_COMMAND(ID_DISP_FILE_NAME, &CMusicPlayerDlg::OnDispFileName)
    ON_COMMAND(ID_DISP_TITLE, &CMusicPlayerDlg::OnDispTitle)
    ON_COMMAND(ID_DISP_ARTIST_TITLE, &CMusicPlayerDlg::OnDispArtistTitle)
    ON_COMMAND(ID_DISP_TITLE_ARTIST, &CMusicPlayerDlg::OnDispTitleArtist)
    ON_COMMAND(ID_MINI_MODE, &CMusicPlayerDlg::OnMiniMode)
    ON_BN_CLICKED(IDC_STOP, &CMusicPlayerDlg::OnBnClickedStop)
    ON_BN_CLICKED(IDC_PREVIOUS, &CMusicPlayerDlg::OnBnClickedPrevious)
    ON_BN_CLICKED(IDC_PLAY_PAUSE, &CMusicPlayerDlg::OnBnClickedPlayPause)
    ON_BN_CLICKED(IDC_NEXT, &CMusicPlayerDlg::OnBnClickedNext)
    ON_COMMAND(ID_RELOAD_LYRIC, &CMusicPlayerDlg::OnReloadLyric)
    ON_COMMAND(ID_SONG_INFO, &CMusicPlayerDlg::OnSongInfo)
    ON_COMMAND(ID_COPY_CURRENT_LYRIC, &CMusicPlayerDlg::OnCopyCurrentLyric)
    ON_COMMAND(ID_COPY_ALL_LYRIC, &CMusicPlayerDlg::OnCopyAllLyric)
    ON_WM_LBUTTONDBLCLK()
    ON_COMMAND(ID_LYRIC_FORWARD, &CMusicPlayerDlg::OnLyricForward)
    ON_COMMAND(ID_LYRIC_DELAY, &CMusicPlayerDlg::OnLyricDelay)
    ON_COMMAND(ID_SAVE_MODIFIED_LYRIC, &CMusicPlayerDlg::OnSaveModifiedLyric)
    ON_COMMAND(ID_EDIT_LYRIC, &CMusicPlayerDlg::OnEditLyric)
    ON_COMMAND(ID_DOWNLOAD_LYRIC, &CMusicPlayerDlg::OnDownloadLyric)
    ON_COMMAND(ID_LYRIC_BATCH_DOWNLOAD, &CMusicPlayerDlg::OnLyricBatchDownload)
    ON_COMMAND(ID_DELETE_LYRIC, &CMusicPlayerDlg::OnDeleteLyric)
    ON_WM_RBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_CTLCOLOR()
    ON_MESSAGE(WM_PLAYLIST_INI_COMPLATE, &CMusicPlayerDlg::OnPlaylistIniComplate)
    ON_MESSAGE(WM_SET_TITLE, &CMusicPlayerDlg::OnSetTitle)
    ON_COMMAND(ID_EQUALIZER, &CMusicPlayerDlg::OnEqualizer)
    ON_COMMAND(ID_EXPLORE_ONLINE, &CMusicPlayerDlg::OnExploreOnline)
    ON_MESSAGE(WM_PLAYLIST_INI_START, &CMusicPlayerDlg::OnPlaylistIniStart)
    ON_COMMAND(ID_BROWSE_LYRIC, &CMusicPlayerDlg::OnBrowseLyric)
    ON_COMMAND(ID_TRANSLATE_TO_SIMPLIFIED_CHINESE, &CMusicPlayerDlg::OnTranslateToSimplifiedChinese)
    ON_COMMAND(ID_TRANSLATE_TO_TRANDITIONAL_CHINESE, &CMusicPlayerDlg::OnTranslateToTranditionalChinese)
    ON_COMMAND(ID_ALBUM_COVER_SAVE_AS, &CMusicPlayerDlg::OnAlbumCoverSaveAs)
    ON_MESSAGE(WM_PATH_SELECTED, &CMusicPlayerDlg::OnPathSelected)
    ON_MESSAGE(WM_CONNOT_PLAY_WARNING, &CMusicPlayerDlg::OnConnotPlayWarning)
    ON_EN_CHANGE(IDC_SEARCH_EDIT, &CMusicPlayerDlg::OnEnChangeSearchEdit)
    ON_BN_CLICKED(IDC_CLEAR_SEARCH_BUTTON, &CMusicPlayerDlg::OnBnClickedClearSearchButton)
    ON_COMMAND(ID_DOWNLOAD_ALBUM_COVER, &CMusicPlayerDlg::OnDownloadAlbumCover)
    ON_MESSAGE(WM_MUSIC_STREAM_OPENED, &CMusicPlayerDlg::OnMusicStreamOpened)
    ON_COMMAND(ID_CURRENT_EXPLORE_ONLINE, &CMusicPlayerDlg::OnCurrentExploreOnline)
    ON_COMMAND(ID_DELETE_ALBUM_COVER, &CMusicPlayerDlg::OnDeleteAlbumCover)
    ON_COMMAND(ID_COPY_FILE_TO, &CMusicPlayerDlg::OnCopyFileTo)
    ON_COMMAND(ID_MOVE_FILE_TO, &CMusicPlayerDlg::OnMoveFileTo)
    ON_MESSAGE(WM_OPEN_FILE_COMMAND_LINE, &CMusicPlayerDlg::OnOpenFileCommandLine)
    ON_COMMAND(ID_FORMAT_CONVERT, &CMusicPlayerDlg::OnFormatConvert)
    ON_COMMAND(ID_FORMAT_CONVERT1, &CMusicPlayerDlg::OnFormatConvert1)
    ON_MESSAGE(WM_SETTINGS_APPLIED, &CMusicPlayerDlg::OnSettingsApplied)
    ON_COMMAND(ID_RECORDER, &CMusicPlayerDlg::OnRecorder)
    ON_MESSAGE(WM_ALBUM_COVER_DOWNLOAD_COMPLETE, &CMusicPlayerDlg::OnAlbumCoverDownloadComplete)
    ON_WM_DWMCOLORIZATIONCOLORCHANGED()
    ON_COMMAND(ID_SUPPORTED_FORMAT, &CMusicPlayerDlg::OnSupportedFormat)
    ON_COMMAND(ID_SWITCH_UI, &CMusicPlayerDlg::OnSwitchUi)
    ON_COMMAND(ID_VOLUME_UP, &CMusicPlayerDlg::OnVolumeUp)
    ON_COMMAND(ID_VOLUME_DOWN, &CMusicPlayerDlg::OnVolumeDown)
    ON_MESSAGE(MY_WM_NOTIFYICON, &CMusicPlayerDlg::OnNotifyicon)
    ON_WM_CLOSE()
    ON_COMMAND(ID_MENU_EXIT, &CMusicPlayerDlg::OnMenuExit)
    ON_COMMAND(ID_MINIMODE_RESTORE, &CMusicPlayerDlg::OnMinimodeRestore)
    ON_WM_APPCOMMAND()
    ON_WM_LBUTTONDOWN()
    ON_COMMAND(ID_SHOW_PLAYLIST, &CMusicPlayerDlg::OnShowPlaylist)
    ON_WM_SETCURSOR()
    ON_WM_MOUSELEAVE()
    ON_COMMAND(ID_SHOW_MENU_BAR, &CMusicPlayerDlg::OnShowMenuBar)
    ON_COMMAND(ID_FULL_SCREEN, &CMusicPlayerDlg::OnFullScreen)
    ON_COMMAND(ID_CREATE_PLAY_SHORTCUT, &CMusicPlayerDlg::OnCreatePlayShortcut)
    ON_COMMAND(ID_LISTEN_STATISTICS, &CMusicPlayerDlg::OnListenStatistics)
    ON_COMMAND(ID_DARK_MODE, &CMusicPlayerDlg::OnDarkMode)
    ON_MESSAGE(WM_MAIN_MENU_POPEDUP, &CMusicPlayerDlg::OnMainMenuPopup)
    ON_COMMAND(ID_ALWAYS_ON_TOP, &CMusicPlayerDlg::OnAlwaysOnTop)
    ON_COMMAND(ID_FLOAT_PLAYLIST, &CMusicPlayerDlg::OnFloatPlaylist)
    ON_COMMAND(ID_DOCKED_PLAYLIST, &CMusicPlayerDlg::OnDockedPlaylist)
    ON_COMMAND(ID_FLOATED_PLAYLIST, &CMusicPlayerDlg::OnFloatedPlaylist)
    ON_MESSAGE(WM_FLOAT_PLAYLIST_CLOSED, &CMusicPlayerDlg::OnFloatPlaylistClosed)
//    ON_COMMAND(ID_FILE_OPEN_PALYLIST, &CMusicPlayerDlg::OnFileOpenPalylist)
    ON_MESSAGE(WM_PLAYLIST_SELECTED, &CMusicPlayerDlg::OnPlaylistSelected)
    ON_COMMAND(ID_PLAYLIST_ADD_FILE, &CMusicPlayerDlg::OnPlaylistAddFile)
    ON_COMMAND(ID_REMOVE_FROM_PLAYLIST, &CMusicPlayerDlg::OnRemoveFromPlaylist)
    ON_COMMAND(ID_EMPTY_PLAYLIST, &CMusicPlayerDlg::OnEmptyPlaylist)
    ON_COMMAND(ID_MOVE_PLAYLIST_ITEM_UP, &CMusicPlayerDlg::OnMovePlaylistItemUp)
    ON_COMMAND(ID_MOVE_PLAYLIST_ITEM_DOWN, &CMusicPlayerDlg::OnMovePlaylistItemDown)
    ON_NOTIFY(NM_CLICK, IDC_PLAYLIST_LIST, &CMusicPlayerDlg::OnNMClickPlaylistList)
    ON_COMMAND(ID_REMOVE_SAME_SONGS, &CMusicPlayerDlg::OnRemoveSameSongs)
    ON_COMMAND(ID_ADD_TO_NEW_PLAYLIST, &CMusicPlayerDlg::OnAddToNewPlaylist)
    ON_COMMAND(ID_TOOL_FILE_RELATE, &CMusicPlayerDlg::OnToolFileRelate)
    ON_COMMAND(ID_PLAYLIST_ADD_FOLDER, &CMusicPlayerDlg::OnPlaylistAddFolder)
    ON_COMMAND(ID_REMOVE_INVALID_ITEMS, &CMusicPlayerDlg::OnRemoveInvalidItems)
    ON_MESSAGE(WM_LIST_ITEM_DRAGGED, &CMusicPlayerDlg::OnListItemDragged)
    ON_COMMAND(ID_ADD_REMOVE_FROM_FAVOURITE, &CMusicPlayerDlg::OnAddRemoveFromFavourite)
    ON_COMMAND(ID_FILE_OPEN_URL, &CMusicPlayerDlg::OnFileOpenUrl)
    ON_COMMAND(ID_PLAYLIST_ADD_URL, &CMusicPlayerDlg::OnPlaylistAddUrl)
    ON_MESSAGE(WM_SET_MENU_STATE, &CMusicPlayerDlg::OnSetMenuState)
    ON_COMMAND(ID_LOCK_DESKTOP_LRYIC, &CMusicPlayerDlg::OnLockDesktopLryic)
    ON_COMMAND(ID_CLOSE_DESKTOP_LYRIC, &CMusicPlayerDlg::OnCloseDesktopLyric)
    ON_COMMAND(ID_LYRIC_DISPLAYED_DOUBLE_LINE, &CMusicPlayerDlg::OnLyricDisplayedDoubleLine)
    ON_COMMAND(ID_LYRIC_BACKGROUND_PENETRATE, &CMusicPlayerDlg::OnLyricBackgroundPenetrate)
END_MESSAGE_MAP()


// CMusicPlayerDlg 消息处理程序

void CMusicPlayerDlg::SaveConfig()
{
    CIniHelper ini(theApp.m_config_path);

    ini.WriteInt(L"config", L"window_width", m_window_width);
    ini.WriteInt(L"config", L"window_hight", m_window_height);
    ini.WriteInt(L"config", L"transparency", theApp.m_app_setting_data.window_transparency);
    ini.WriteBool(L"config", L"narrow_mode", theApp.m_ui_data.narrow_mode);
    ini.WriteBool(L"config", L"show_translate", theApp.m_ui_data.show_translate);
    ini.WriteBool(L"config", L"show_playlist", theApp.m_ui_data.show_playlist);
    ini.WriteBool(L"config", L"show_menu_bar", theApp.m_ui_data.show_menu_bar);
    ini.WriteBool(L"config", L"float_playlist", theApp.m_nc_setting_data.float_playlist);
    ini.WriteInt(L"config", L"float_playlist_width", theApp.m_nc_setting_data.playlist_size.cx);
    ini.WriteInt(L"config", L"float_playlist_height", theApp.m_nc_setting_data.playlist_size.cy);
    ini.WriteBool(L"config", L"playlist_btn_for_float_playlist", theApp.m_nc_setting_data.playlist_btn_for_float_playlist);

    ini.WriteInt(L"config", L"theme_color", theApp.m_app_setting_data.theme_color.original_color);
    ini.WriteBool(L"config", L"theme_color_follow_system", theApp.m_app_setting_data.theme_color_follow_system);
    ini.WriteInt(L"config", L"playlist_display_format", static_cast<int>(theApp.m_ui_data.display_format));
    ini.WriteBool(L"config", L"show_lyric_in_cortana", theApp.m_lyric_setting_data.cortana_info_enable);
    ini.WriteBool(L"config", L"cortana_show_lyric", theApp.m_lyric_setting_data.cortana_show_lyric);
    ini.WriteBool(L"config", L"save_lyric_in_offset", theApp.m_lyric_setting_data.save_lyric_in_offset);
    ini.WriteString(L"config", L"font", theApp.m_app_setting_data.lyric_font.name);
    ini.WriteInt(L"config", L"font_size", theApp.m_app_setting_data.lyric_font.size);
    ini.WriteInt(L"config", L"font_style", theApp.m_app_setting_data.lyric_font.style.ToInt());
    ini.WriteInt(L"config", L"lyric_line_space", theApp.m_app_setting_data.lyric_line_space);
    ini.WriteInt(L"config", L"spectrum_height", theApp.m_app_setting_data.sprctrum_height);
    ini.WriteBool(L"config", L"cortana_lyric_double_line", theApp.m_lyric_setting_data.cortana_lyric_double_line);
    ini.WriteBool(L"config", L"show_spectrum", theApp.m_app_setting_data.show_spectrum);
    ini.WriteBool(L"config", L"show_album_cover", theApp.m_app_setting_data.show_album_cover);
    ini.WriteInt(L"config", L"album_cover_fit", static_cast<int>(theApp.m_app_setting_data.album_cover_fit));
    ini.WriteBool(L"config", L"album_cover_as_background", theApp.m_app_setting_data.album_cover_as_background);
    ini.WriteBool(L"config", L"cortana_show_album_cover", theApp.m_lyric_setting_data.cortana_show_album_cover);
    ini.WriteBool(L"config", L"cortana_icon_beat", theApp.m_lyric_setting_data.cortana_icon_beat);
    ini.WriteBool(L"config", L"cortana_lyric_compatible_mode", theApp.m_lyric_setting_data.cortana_lyric_compatible_mode);
    ini.WriteString(L"config", L"cortana_font", theApp.m_lyric_setting_data.cortana_font.name);
    ini.WriteInt(L"config", L"cortana_font_size", theApp.m_lyric_setting_data.cortana_font.size);
    ini.WriteInt(L"config", L"cortana_font_style", theApp.m_lyric_setting_data.cortana_font.style.ToInt());
    ini.WriteBool(L"config", L"cortana_lyric_keep_display", theApp.m_lyric_setting_data.cortana_lyric_keep_display);
    ini.WriteBool(L"config", L"cortana_show_spectrum", theApp.m_lyric_setting_data.cortana_show_spectrum);
    
	ini.WriteBool(L"desktop_lyric", L"show_desktop_lyric", theApp.m_lyric_setting_data.show_desktop_lyric);
	ini.WriteString(L"desktop_lyric", L"font_name", theApp.m_lyric_setting_data.desktop_lyric_data.lyric_font.name);
	ini.WriteInt(L"desktop_lyric", L"font_size", theApp.m_lyric_setting_data.desktop_lyric_data.lyric_font.size);
	ini.WriteInt(L"desktop_lyric", L"font_style", theApp.m_lyric_setting_data.desktop_lyric_data.lyric_font.style.ToInt());
	ini.WriteInt(L"desktop_lyric", L"text_color1", theApp.m_lyric_setting_data.desktop_lyric_data.text_color1);
	ini.WriteInt(L"desktop_lyric", L"text_color2", theApp.m_lyric_setting_data.desktop_lyric_data.text_color2);
	ini.WriteInt(L"desktop_lyric", L"text_gradient", theApp.m_lyric_setting_data.desktop_lyric_data.text_gradient);
	ini.WriteInt(L"desktop_lyric", L"highlight_color1", theApp.m_lyric_setting_data.desktop_lyric_data.highlight_color1);
	ini.WriteInt(L"desktop_lyric", L"highlight_color2", theApp.m_lyric_setting_data.desktop_lyric_data.highlight_color2);
	ini.WriteInt(L"desktop_lyric", L"highlight_gradient", theApp.m_lyric_setting_data.desktop_lyric_data.highlight_gradient);
	ini.WriteBool(L"desktop_lyric", L"lock_desktop_lyric", theApp.m_lyric_setting_data.desktop_lyric_data.lock_desktop_lyric);
	ini.WriteBool(L"desktop_lyric", L"lyric_double_line", theApp.m_lyric_setting_data.desktop_lyric_data.lyric_double_line);
	ini.WriteBool(L"desktop_lyric", L"hide_lyric_window_without_lyric", theApp.m_lyric_setting_data.desktop_lyric_data.hide_lyric_window_without_lyric);
	ini.WriteBool(L"desktop_lyric", L"hide_lyric_window_when_paused", theApp.m_lyric_setting_data.desktop_lyric_data.hide_lyric_window_when_paused);
	ini.WriteBool(L"desktop_lyric", L"lyric_background_penetrate", theApp.m_lyric_setting_data.desktop_lyric_data.lyric_background_penetrate);
	ini.WriteInt(L"desktop_lyric", L"opacity", theApp.m_lyric_setting_data.desktop_lyric_data.opacity);
	ini.WriteInt(L"desktop_lyric", L"position_x", m_desktop_lyric_pos.x);
	ini.WriteInt(L"desktop_lyric", L"position_y", m_desktop_lyric_pos.y);
	ini.WriteInt(L"desktop_lyric", L"width", m_desktop_lyric_size.cx);
	ini.WriteInt(L"desktop_lyric", L"height", m_desktop_lyric_size.cy);

    ini.WriteBool(L"config", L"background_gauss_blur", theApp.m_app_setting_data.background_gauss_blur);
    ini.WriteInt(L"config", L"gauss_blur_radius", theApp.m_app_setting_data.gauss_blur_radius);
    ini.WriteBool(L"config", L"lyric_background", theApp.m_app_setting_data.lyric_background);
    ini.WriteBool(L"config", L"dark_mode", theApp.m_app_setting_data.dark_mode);
    ini.WriteInt(L"config", L"background_transparency", theApp.m_app_setting_data.background_transparency);
    ini.WriteBool(L"config", L"use_out_image", theApp.m_app_setting_data.use_out_image);
    ini.WriteBool(L"config", L"use_inner_image_first", theApp.m_app_setting_data.use_inner_image_first);

    ini.WriteInt(L"config", L"volum_step", theApp.m_nc_setting_data.volum_step);
    ini.WriteInt(L"config", L"mouse_volum_step", theApp.m_nc_setting_data.mouse_volum_step);
    ini.WriteInt(L"config", L"cortana_back_color", theApp.m_lyric_setting_data.cortana_color);
    ini.WriteInt(L"config", L"volume_map", theApp.m_nc_setting_data.volume_map);
    ini.WriteBool(L"config", L"show_cover_tip", theApp.m_nc_setting_data.show_cover_tip);
    ini.WriteBool(L"config", L"always_on_top", theApp.m_nc_setting_data.always_on_top);

    ini.WriteBool(L"other", L"no_sf2_warning", theApp.m_nc_setting_data.no_sf2_warning);
    ini.WriteBool(L"other", L"show_hide_menu_bar_tip", theApp.m_nc_setting_data.show_hide_menu_bar_tip);
    ini.WriteBool(L"other", L"cortana_opaque", theApp.m_lyric_setting_data.cortana_opaque);
    ini.WriteInt(L"other", L"cortana_transparent_color", theApp.m_nc_setting_data.cortana_transparent_color);

    ini.WriteBool(L"general", L"id3v2_first", theApp.m_general_setting_data.id3v2_first);
    ini.WriteBool(L"general", L"auto_download_lyric", theApp.m_general_setting_data.auto_download_lyric);
    ini.WriteBool(L"general", L"auto_download_album_cover", theApp.m_general_setting_data.auto_download_album_cover);
    ini.WriteBool(L"general", L"auto_download_only_tag_full", theApp.m_general_setting_data.auto_download_only_tag_full);
    ini.WriteString(L"general", L"sf2_path", theApp.m_general_setting_data.sf2_path);
    ini.WriteBool(L"general", L"midi_use_inner_lyric", theApp.m_general_setting_data.midi_use_inner_lyric);
    ini.WriteBool(L"general", L"minimize_to_notify_icon", theApp.m_general_setting_data.minimize_to_notify_icon);

    ini.WriteBool(L"config", L"stop_when_error", theApp.m_play_setting_data.stop_when_error);
    ini.WriteBool(L"config", L"auto_play_when_start", theApp.m_play_setting_data.auto_play_when_start);
    ini.WriteBool(L"config", L"show_taskbar_progress", theApp.m_play_setting_data.show_taskbar_progress);
    ini.WriteBool(L"config", L"show_playstate_icon", theApp.m_play_setting_data.show_playstate_icon);
    ini.WriteBool(L"config", L"fade_effect", theApp.m_play_setting_data.fade_effect);
    ini.WriteInt(L"config", L"fade_time", theApp.m_play_setting_data.fade_time);
    ini.WriteString(L"config", L"output_device", theApp.m_play_setting_data.output_device);
    ini.WriteBool(L"config", L"use_mci", theApp.m_play_setting_data.use_mci);

    int ui_selected;
    if (m_pUI == &m_ui)
        ui_selected = 0;
    else
        ui_selected = 1;
    ini.WriteInt(L"config", L"UI_selected", ui_selected);

    //保存热键设置
    ini.WriteBool(L"hot_key", L"hot_key_enable", theApp.m_hot_key_setting_data.hot_key_enable);
    theApp.m_hot_key.SaveToTni(ini);

    ini.Save();
}

void CMusicPlayerDlg::LoadConfig()
{
    CIniHelper ini(theApp.m_config_path);

    m_window_width = ini.GetInt(L"config", L"window_width", theApp.DPI(698));
    m_window_height = ini.GetInt(L"config", L"window_hight", theApp.DPI(565));
    theApp.m_app_setting_data.window_transparency = ini.GetInt(L"config", L"transparency", 100);
    theApp.m_ui_data.narrow_mode = ini.GetBool(L"config", L"narrow_mode", false);
    theApp.m_ui_data.show_translate = ini.GetBool(L"config", L"show_translate", true);
    theApp.m_ui_data.show_playlist = ini.GetBool(L"config", L"show_playlist", true);
    theApp.m_ui_data.show_menu_bar = ini.GetBool(L"config", L"show_menu_bar", true);
    theApp.m_nc_setting_data.float_playlist = ini.GetBool(L"config", L"float_playlist", false);
    theApp.m_nc_setting_data.playlist_size.cx = ini.GetInt(L"config", L"float_playlist_width", theApp.DPI(320));
    theApp.m_nc_setting_data.playlist_size.cy = ini.GetInt(L"config", L"float_playlist_height", theApp.DPI(530));
    theApp.m_nc_setting_data.playlist_btn_for_float_playlist = ini.GetBool(L"config", L"playlist_btn_for_float_playlist", false);

    theApp.m_app_setting_data.theme_color.original_color = ini.GetInt(L"config", L"theme_color", 16760187);
    theApp.m_app_setting_data.theme_color_follow_system = ini.GetBool(L"config", L"theme_color_follow_system", true);
    theApp.m_ui_data.display_format = static_cast<DisplayFormat>(ini.GetInt(L"config", L"playlist_display_format", 2));
    theApp.m_lyric_setting_data.cortana_show_lyric = ini.GetBool(L"config", L"cortana_show_lyric", true);
    theApp.m_lyric_setting_data.cortana_info_enable = ini.GetBool(L"config", L"show_lyric_in_cortana", false);
    theApp.m_lyric_setting_data.save_lyric_in_offset = ini.GetBool(L"config", L"save_lyric_in_offset", false);
    theApp.m_app_setting_data.lyric_font.name = ini.GetString(L"config", L"font", CCommon::LoadText(IDS_DEFAULT_FONT));
    theApp.m_app_setting_data.lyric_font.size = ini.GetInt(L"config", L"font_size", 11);
    theApp.m_app_setting_data.lyric_font.style.FromInt(ini.GetInt(L"config", L"font_style", 0));
    theApp.m_app_setting_data.lyric_line_space = ini.GetInt(L"config", L"lyric_line_space", 4);
    theApp.m_app_setting_data.sprctrum_height = ini.GetInt(L"config", L"spectrum_height", 80);
    theApp.m_lyric_setting_data.cortana_lyric_double_line = ini.GetBool(L"config", L"cortana_lyric_double_line", true);
    theApp.m_app_setting_data.show_spectrum = ini.GetBool(L"config", L"show_spectrum", 1);
    theApp.m_app_setting_data.show_album_cover = ini.GetBool(L"config", L"show_album_cover", 1);
    theApp.m_app_setting_data.album_cover_fit = static_cast<CDrawCommon::StretchMode>(ini.GetInt(L"config", L"album_cover_fit", 1));
    theApp.m_app_setting_data.album_cover_as_background = ini.GetBool(L"config", L"album_cover_as_background", true);
    theApp.m_lyric_setting_data.cortana_show_album_cover = ini.GetBool(L"config", L"cortana_show_album_cover", 1);
    theApp.m_lyric_setting_data.cortana_icon_beat = ini.GetBool(L"config", L"cortana_icon_beat", 0);
    theApp.m_lyric_setting_data.cortana_lyric_compatible_mode = ini.GetBool(L"config", L"cortana_lyric_compatible_mode", false);
    theApp.m_lyric_setting_data.cortana_font.name = ini.GetString(L"config", L"cortana_font", CCommon::LoadText(IDS_DEFAULT_FONT));
    theApp.m_lyric_setting_data.cortana_font.size = ini.GetInt(L"config", L"cortana_font_size", 11);
    theApp.m_lyric_setting_data.cortana_font.style.FromInt(ini.GetInt(L"config", L"cortana_font_style", 0));
    theApp.m_lyric_setting_data.cortana_lyric_keep_display = ini.GetBool(L"config", L"cortana_lyric_keep_display", false);
    theApp.m_lyric_setting_data.cortana_show_spectrum = ini.GetBool(L"config", L"cortana_show_spectrum", false);

	theApp.m_lyric_setting_data.show_desktop_lyric = ini.GetBool(L"desktop_lyric", L"show_desktop_lyric", false);
	theApp.m_lyric_setting_data.desktop_lyric_data.lyric_font.name = ini.GetString(L"desktop_lyric", L"font_name", CCommon::LoadText(IDS_DEFAULT_FONT));
	theApp.m_lyric_setting_data.desktop_lyric_data.lyric_font.size = ini.GetInt(L"desktop_lyric", L"font_size", 30);
	theApp.m_lyric_setting_data.desktop_lyric_data.lyric_font.style.FromInt(ini.GetInt(L"desktop_lyric", L"font_style", 0));
	theApp.m_lyric_setting_data.desktop_lyric_data.text_color1 = ini.GetInt(L"desktop_lyric", L"text_color1", RGB(37, 152, 10));
	theApp.m_lyric_setting_data.desktop_lyric_data.text_color2 = ini.GetInt(L"desktop_lyric", L"text_color2", RGB(129, 249, 0));
	theApp.m_lyric_setting_data.desktop_lyric_data.text_gradient = ini.GetInt(L"desktop_lyric", L"text_gradient", 1);
	theApp.m_lyric_setting_data.desktop_lyric_data.highlight_color1 = ini.GetInt(L"desktop_lyric", L"highlight_color1", RGB(253, 232, 0));
	theApp.m_lyric_setting_data.desktop_lyric_data.highlight_color2 = ini.GetInt(L"desktop_lyric", L"highlight_color2", RGB(255, 120, 0));
	theApp.m_lyric_setting_data.desktop_lyric_data.highlight_gradient = ini.GetInt(L"desktop_lyric", L"highlight_gradient", 2);
	theApp.m_lyric_setting_data.desktop_lyric_data.lock_desktop_lyric = ini.GetBool(L"desktop_lyric", L"lock_desktop_lyric", false);
	theApp.m_lyric_setting_data.desktop_lyric_data.lyric_double_line = ini.GetBool(L"desktop_lyric", L"lyric_double_line", false);
	theApp.m_lyric_setting_data.desktop_lyric_data.hide_lyric_window_without_lyric = ini.GetBool(L"desktop_lyric", L"hide_lyric_window_without_lyric", false);
	theApp.m_lyric_setting_data.desktop_lyric_data.hide_lyric_window_when_paused = ini.GetBool(L"desktop_lyric", L"hide_lyric_window_when_paused", false);
	theApp.m_lyric_setting_data.desktop_lyric_data.lyric_background_penetrate = ini.GetBool(L"desktop_lyric", L"lyric_background_penetrate", false);
	theApp.m_lyric_setting_data.desktop_lyric_data.opacity = ini.GetInt(L"desktop_lyric", L"opacity", 100);
    m_desktop_lyric_pos.x = ini.GetInt(L"desktop_lyric", L"position_x", -1);
    m_desktop_lyric_pos.y = ini.GetInt(L"desktop_lyric", L"position_y", -1);
    m_desktop_lyric_size.cx = ini.GetInt(L"desktop_lyric", L"width", 0);
    m_desktop_lyric_size.cy = ini.GetInt(L"desktop_lyric", L"height", 0);

    theApp.m_app_setting_data.background_gauss_blur = ini.GetBool(L"config", L"background_gauss_blur", true);
    theApp.m_app_setting_data.gauss_blur_radius = ini.GetInt(L"config", L"gauss_blur_radius", 60);
    theApp.m_app_setting_data.lyric_background = ini.GetBool(L"config", L"lyric_background", false);
    theApp.m_app_setting_data.dark_mode = ini.GetBool(L"config", L"dark_mode", false);
    theApp.m_app_setting_data.background_transparency = ini.GetInt(L"config", L"background_transparency", 80);
    theApp.m_app_setting_data.use_out_image = ini.GetBool(L"config", L"use_out_image", 0);
    theApp.m_app_setting_data.use_inner_image_first = ini.GetBool(L"config", L"use_inner_image_first", true);

    theApp.m_nc_setting_data.volum_step = ini.GetInt(L"config", L"volum_step", 3);
    theApp.m_nc_setting_data.mouse_volum_step = ini.GetInt(L"config", L"mouse_volum_step", 2);
    theApp.m_lyric_setting_data.cortana_color = ini.GetInt(L"config", L"cortana_back_color", 0);
    theApp.m_nc_setting_data.volume_map = ini.GetInt(L"config", L"volume_map", 100);
    theApp.m_nc_setting_data.show_cover_tip = ini.GetBool(L"config", L"show_cover_tip", false);
    theApp.m_nc_setting_data.always_on_top = ini.GetBool(L"config", L"always_on_top", false);

    theApp.m_nc_setting_data.no_sf2_warning = ini.GetBool(L"other", L"no_sf2_warning", true);
    theApp.m_nc_setting_data.show_hide_menu_bar_tip = ini.GetBool(L"other", L"show_hide_menu_bar_tip", true);
    theApp.m_lyric_setting_data.cortana_opaque = ini.GetBool(L"other", L"cortana_opaque", false);
    theApp.m_nc_setting_data.cortana_transparent_color = ini.GetInt(L"other", L"cortana_transparent_color", RGB(255, 0, 255));
	theApp.m_nc_setting_data.default_osu_img = ini.GetString(L"other", L"default_osu_img", L"");

    theApp.m_general_setting_data.id3v2_first = ini.GetBool(L"general", L"id3v2_first", 1);
    theApp.m_general_setting_data.auto_download_lyric = ini.GetBool(L"general", L"auto_download_lyric", 1);
    theApp.m_general_setting_data.auto_download_album_cover = ini.GetBool(L"general", L"auto_download_album_cover", 1);
    theApp.m_general_setting_data.auto_download_only_tag_full = ini.GetBool(L"general", L"auto_download_only_tag_full", 1);
    theApp.m_general_setting_data.sf2_path = ini.GetString(L"general", L"sf2_path", L"");
    theApp.m_general_setting_data.midi_use_inner_lyric = ini.GetBool(L"general", L"midi_use_inner_lyric", 0);
    theApp.m_general_setting_data.minimize_to_notify_icon = ini.GetBool(L"general", L"minimize_to_notify_icon", false);

    theApp.m_play_setting_data.stop_when_error = ini.GetBool(L"config", L"stop_when_error", true);
    theApp.m_play_setting_data.auto_play_when_start = ini.GetBool(L"config", L"auto_play_when_start", false);
    theApp.m_play_setting_data.show_taskbar_progress = ini.GetBool(L"config", L"show_taskbar_progress", true);
    theApp.m_play_setting_data.show_playstate_icon = ini.GetBool(L"config", L"show_playstate_icon", true);
    theApp.m_play_setting_data.fade_effect = ini.GetBool(L"config", L"fade_effect", true);
    theApp.m_play_setting_data.fade_time = ini.GetInt(L"config", L"fade_time", 500);
    if (theApp.m_play_setting_data.fade_time < 0)
        theApp.m_play_setting_data.fade_time = 0;
    if (theApp.m_play_setting_data.fade_time > 2000)
        theApp.m_play_setting_data.fade_time = 2000;
    theApp.m_play_setting_data.output_device = ini.GetString(L"config", L"output_device", L"");
    theApp.m_play_setting_data.use_mci = ini.GetBool(L"config", L"use_mci", false);

    int ui_selected = ini.GetInt(L"config", L"UI_selected", 1);
    if (ui_selected == 0)
        m_pUI = &m_ui;
    else
        m_pUI = &m_ui2;

    //载入热键设置
    theApp.m_hot_key_setting_data.hot_key_enable = ini.GetBool(L"hot_key", L"hot_key_enable", true);
    theApp.m_hot_key.LoadFromIni(ini);

}

void CMusicPlayerDlg::SetTransparency()
{
    CCommon::SetWindowOpacity(m_hWnd, theApp.m_app_setting_data.window_transparency);
}

void CMusicPlayerDlg::SetDesptopLyricTransparency()
{
    m_desktop_lyric.SetLyricOpacity(theApp.m_lyric_setting_data.desktop_lyric_data.opacity);
}

void CMusicPlayerDlg::DrawInfo(bool reset)
{
    if (!IsIconic() && IsWindowVisible())		//窗口最小化或隐藏时不绘制，以降低CPU利用率
        m_pUI->DrawInfo(reset);
}

void CMusicPlayerDlg::SetPlaylistSize(int cx, int cy)
{
    //设置播放列表大小
    if (!theApp.m_ui_data.narrow_mode)
    {
        m_playlist_list.MoveWindow(cx / 2 + m_layout.margin, m_layout.search_edit_height + m_layout.path_edit_height + m_layout.margin,
                                   cx / 2 - 2 * m_layout.margin, cy - m_layout.search_edit_height - m_layout.path_edit_height - 2 * m_layout.margin);
    }
    else
    {
        m_playlist_list.MoveWindow(m_layout.margin, m_ui.DrawAreaHeight() + m_layout.search_edit_height + m_layout.path_edit_height,
                                   cx - 2 * m_layout.margin, cy - m_ui.DrawAreaHeight() - m_layout.search_edit_height - m_layout.path_edit_height - m_layout.margin);
    }
    m_playlist_list.AdjustColumnWidth();

    //设置“当前路径”static控件大小
    CRect rect_static;
    m_path_static.GetWindowRect(rect_static);
    rect_static.bottom = rect_static.top + m_layout.path_edit_height - 2 * m_layout.margin;
    //CDrawCommon draw;
    //int width = theApp.DPI(70);
    //if (m_pDC != nullptr)
    //{
    //    draw.Create(m_pDC, this);
    //    CString str = m_path_static.GetWindowText();
    //    width = draw.GetTextExtent(str).cx + theApp.DPI(8);
    //}
    //rect_static.right = rect_static.left + width;
    if (!theApp.m_ui_data.narrow_mode)
        rect_static.MoveToXY(cx / 2 + m_layout.margin, m_layout.margin);
    else
        rect_static.MoveToXY(m_layout.margin, m_ui.DrawAreaHeight());
    m_path_static.MoveWindow(rect_static);

    //设置“当前路径”edit控件大小
    CRect rect_edit;
    m_path_edit.GetWindowRect(rect_edit);
    if (!theApp.m_ui_data.narrow_mode)
    {
        rect_edit.right = rect_edit.left + (cx / 2 - 3 * m_layout.margin - rect_static.Width() - m_layout.select_folder_width);
        rect_edit.MoveToXY(cx / 2 + m_layout.margin + rect_static.Width(), m_layout.margin);
    }
    else
    {
        rect_edit.right = rect_edit.left + (cx - 3 * m_layout.margin - rect_static.Width() - m_layout.select_folder_width);
        rect_edit.MoveToXY(m_layout.margin + rect_static.Width(), m_ui.DrawAreaHeight());
    }
    m_path_edit.MoveWindow(rect_edit);

    //设置“选择文件夹”按钮的大小和位置
    CRect rect_select_folder{ rect_edit };
    rect_select_folder.left = rect_edit.right + m_layout.margin;
    rect_select_folder.right = cx - m_layout.margin;
    rect_select_folder.top--;
    rect_select_folder.bottom++;
    m_set_path_button.MoveWindow(rect_select_folder);

    //设置歌曲搜索框的大小和位置
    CRect rect_search;
    m_search_edit.GetWindowRect(rect_search);
    if (!theApp.m_ui_data.narrow_mode)
    {
        rect_search.right = rect_search.left + (cx / 2 - 2 * m_layout.margin - m_layout.margin - rect_search.Height());
        rect_search.MoveToXY(cx / 2 + m_layout.margin, m_layout.path_edit_height + theApp.DPI(1));
    }
    else
    {
        rect_search.right = rect_search.left + (cx - 2 * m_layout.margin - m_layout.margin - rect_search.Height());
        rect_search.MoveToXY(m_layout.margin, m_ui.DrawAreaHeight() + m_layout.path_edit_height - theApp.DPI(3));
    }
    m_search_edit.MoveWindow(rect_search);
    //设置清除搜索按钮的大小和位置
    CRect rect_clear{};
    rect_clear.right = rect_clear.bottom = rect_search.Height();
    //if (!theApp.m_ui_data.narrow_mode)
    rect_clear.MoveToXY(rect_search.right + m_layout.margin, rect_search.top);
    m_clear_search_button.MoveWindow(rect_clear);
    m_clear_search_button.Invalidate();
}

void CMusicPlayerDlg::SetAlwaysOnTop()
{
    if (theApp.m_nc_setting_data.always_on_top)
        SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);			//设置置顶
    else
        SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);		//取消置顶
}

void CMusicPlayerDlg::ShowPlayList(bool highlight_visible)
{
    m_playlist_list.ShowPlaylist(theApp.m_ui_data.display_format);
    m_playlist_list.SetCurSel(-1);
    //设置播放列表中突出显示的项目
    SetPlayListColor(highlight_visible);
    //显示当前路径
    m_path_edit.SetWindowTextW(CPlayer::GetInstance().GetCurrentFolderOrPlaylistName().c_str());
    CStaticEx* pStatic{};
    if (m_pFloatPlaylistDlg->GetSafeHwnd() == NULL)
        pStatic = &m_path_static;
    else
        pStatic = &m_pFloatPlaylistDlg->GetPathStatic();
    if (CPlayer::GetInstance().IsPlaylistMode())
        pStatic->SetWindowText(CCommon::LoadText(IDS_PLAYLIST, _T(":")));
    else
        pStatic->SetWindowText(CCommon::LoadText(IDS_CURRENT_FOLDER, _T(":")));

    if (m_miniModeDlg.m_hWnd != NULL)
    {
        m_miniModeDlg.ShowPlaylist();
    }

    if (theApp.m_nc_setting_data.float_playlist && m_pFloatPlaylistDlg != nullptr)
    {
        m_pFloatPlaylistDlg->RefreshData();
    }
}

void CMusicPlayerDlg::SetPlayListColor(bool highlight_visible)
{
    m_playlist_list.SetHightItem(CPlayer::GetInstance().GetIndex());
    //m_playlist_list.SetColor(theApp.m_app_setting_data.theme_color);
    m_playlist_list.Invalidate(FALSE);
    if(highlight_visible)
        m_playlist_list.EnsureVisible(CPlayer::GetInstance().GetIndex(), FALSE);

    if (theApp.m_nc_setting_data.float_playlist && m_pFloatPlaylistDlg != nullptr)
    {
        m_pFloatPlaylistDlg->RefreshState();
    }
}

void CMusicPlayerDlg::SwitchTrack()
{
    //当切换正在播放的歌曲时设置播放列表中突出显示的项目
    SetPlayListColor();
    if (m_miniModeDlg.m_hWnd != NULL)
    {
        m_miniModeDlg.SetPlayListColor();
        //m_miniModeDlg.RePaint();
        m_miniModeDlg.Invalidate(FALSE);
    }
    //切换歌曲时如果当前歌曲的时间没有显示，则显示出来
    CString song_length_str;
    int index{ CPlayer::GetInstance().GetIndex() };
    song_length_str = m_playlist_list.GetItemText(index, 2);
    if (song_length_str == _T("-:--") && !CPlayer::GetInstance().GetAllSongLength(index).isZero())
    {
        m_playlist_list.SetItemText(index, 2, CPlayer::GetInstance().GetAllSongLength(index).toString().c_str());
    }

    DrawInfo(true);

    m_ui.UpdateSongInfoToolTip();
    m_ui2.UpdateSongInfoToolTip();
}

void CMusicPlayerDlg::SetPlaylistVisible()
{
    int cmdShow = (theApp.m_ui_data.show_playlist ? SW_SHOW : SW_HIDE);
    m_playlist_list.ShowWindow(cmdShow);
    m_path_static.ShowWindow(cmdShow);
    m_path_edit.ShowWindow(cmdShow);
    m_search_edit.ShowWindow(cmdShow);
    m_clear_search_button.ShowWindow(cmdShow);
    m_set_path_button.ShowWindow(cmdShow);
}

void CMusicPlayerDlg::SetMenubarVisible()
{
    if (theApp.m_ui_data.show_menu_bar)
    {
        SetMenu(&theApp.m_menu_set.m_main_menu);
    }
    else
    {
        SetMenu(nullptr);
    }
}

void CMusicPlayerDlg::UpdateTaskBarProgress()
{
#ifndef COMPILE_IN_WIN_XP
    if (IsTaskbarListEnable())
    {
        //根据播放状态设置任务栏状态和进度
        if (theApp.m_play_setting_data.show_taskbar_progress)
        {
            int position, length;
            position = CPlayer::GetInstance().GetCurrentPosition();
            length = CPlayer::GetInstance().GetSongLength();
            if (position == 0) position = 1;
            if (length == 0) length = 1;
            if (CPlayer::GetInstance().IsError())
                m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_ERROR);
            else if (CPlayer::GetInstance().IsPlaying())
                m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_INDETERMINATE);
            else
                m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_PAUSED);
            m_pTaskbar->SetProgressValue(this->GetSafeHwnd(), position, length);
        }
        else
        {
            m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_NOPROGRESS);
        }
    }
#endif
}

void CMusicPlayerDlg::UpdatePlayPauseButton()
{
    if (CPlayer::GetInstance().IsPlaying() && !CPlayer::GetInstance().IsError())
    {
#ifndef COMPILE_IN_WIN_XP
        if (IsTaskbarListEnable())
        {
            //更新任务栏缩略图上“播放/暂停”的图标
            m_thumbButton[1].hIcon = theApp.m_icon_set.pause.GetIcon();
            wcscpy_s(m_thumbButton[1].szTip, CCommon::LoadText(IDS_PAUSE));
            //更新任务按钮上的播放状态图标
            if (theApp.m_play_setting_data.show_playstate_icon)
                m_pTaskbar->SetOverlayIcon(m_hWnd, theApp.m_icon_set.play.GetIcon(), L"");
            else
                m_pTaskbar->SetOverlayIcon(m_hWnd, NULL, L"");
        }
#endif
    }
    else
    {
#ifndef COMPILE_IN_WIN_XP
        if (IsTaskbarListEnable())
        {
            //更新任务栏缩略图上“播放/暂停”的图标
            m_thumbButton[1].hIcon = theApp.m_icon_set.play.GetIcon();
            wcscpy_s(m_thumbButton[1].szTip, CCommon::LoadText(IDS_PLAY));
            //更新任务按钮上的播放状态图标
            if (theApp.m_play_setting_data.show_playstate_icon && CPlayer::GetInstance().GetPlayingState2() == 1)
                m_pTaskbar->SetOverlayIcon(m_hWnd, theApp.m_icon_set.pause.GetIcon(), L"");
            else
                m_pTaskbar->SetOverlayIcon(m_hWnd, NULL, L"");
        }
#endif
    }
#ifndef COMPILE_IN_WIN_XP
    if (IsTaskbarListEnable())
        m_pTaskbar->ThumbBarUpdateButtons(m_hWnd, 3, m_thumbButton);
#endif
    if (m_miniModeDlg.m_hWnd != NULL)
        m_miniModeDlg.UpdatePlayPauseButton();

    m_pUI->UpdatePlayPauseButtonTip();
}

void CMusicPlayerDlg::SetThumbnailClipArea()
{
#ifndef COMPILE_IN_WIN_XP
    if (IsTaskbarListEnable())
    {
        if (m_pTaskbar != nullptr && m_pUI != nullptr)
            m_pTaskbar->SetThumbnailClip(m_hWnd, m_pUI->GetThumbnailClipArea());
    }
#endif
}

void CMusicPlayerDlg::EnablePlaylist(bool enable)
{
    m_playlist_list.EnableWindow(enable);
    m_search_edit.EnableWindow(enable);
    m_clear_search_button.EnableWindow(enable);
    m_set_path_button.EnableWindow(enable);

    if (m_pFloatPlaylistDlg->GetSafeHwnd() != NULL)
        m_pFloatPlaylistDlg->EnableControl(enable);
    if (m_miniModeDlg.GetSafeHwnd() != NULL)
        m_miniModeDlg.GetPlaylistCtrl().EnableWindow(enable);
}


void CMusicPlayerDlg::CreateDesktopShortcut()
{
    //如果目录下没有recent_path和song_data文件，就判断为是第一次运行程序，提示用户是否创建桌面快捷方式
    if (!CCommon::FileExist(theApp.m_song_data_path) && !CCommon::FileExist(theApp.m_recent_path_dat_path))
    {
        wstring shortcut_path;

        if (MessageBox(CCommon::LoadText(IDS_CREATE_SHORTCUT_INFO), NULL, MB_ICONQUESTION | MB_YESNO) == IDYES)
        {
            if (CCommon::CreateFileShortcut(theApp.m_desktop_path.c_str(), NULL, _T("MusicPlayer2.lnk")))
            {
                CString info;
                info = CCommon::LoadTextFormat(IDS_SHORTCUT_CREATED, { theApp.m_desktop_path });
                MessageBox(info, NULL, MB_ICONINFORMATION);
            }
            else
            {
                MessageBox(CCommon::LoadText(IDS_SHORTCUT_CREAT_FAILED), NULL, MB_ICONWARNING);
            }
        }
    }
}

void CMusicPlayerDlg::ApplySettings(const COptionsDlg& optionDlg)
{
    //获取选项设置对话框中的设置数据
    if (theApp.m_lyric_setting_data.cortana_info_enable == true && optionDlg.m_tab1_dlg.m_data.cortana_info_enable == false)	//如果在选项中关闭了“在Cortana搜索框中显示歌词”的选项，则重置Cortana搜索框的文本
        m_cortana_lyric.ResetCortanaText();

    bool reload_sf2{ theApp.m_general_setting_data.sf2_path != optionDlg.m_tab3_dlg.m_data.sf2_path };
    bool gauss_blur_changed{ theApp.m_app_setting_data.background_gauss_blur != optionDlg.m_tab2_dlg.m_data.background_gauss_blur
                             || theApp.m_app_setting_data.gauss_blur_radius != optionDlg.m_tab2_dlg.m_data.gauss_blur_radius
                             || theApp.m_app_setting_data.album_cover_as_background != optionDlg.m_tab2_dlg.m_data.album_cover_as_background };
    bool output_device_changed{ theApp.m_play_setting_data.output_device != optionDlg.m_tab4_dlg.m_data.output_device };
    bool player_core_changed{ theApp.m_play_setting_data.use_mci != optionDlg.m_tab4_dlg.m_data.use_mci };

    theApp.m_lyric_setting_data = optionDlg.m_tab1_dlg.m_data;
    theApp.m_app_setting_data = optionDlg.m_tab2_dlg.m_data;
    theApp.m_general_setting_data = optionDlg.m_tab3_dlg.m_data;
    theApp.m_play_setting_data = optionDlg.m_tab4_dlg.m_data;
    theApp.m_hot_key.FromHotkeyGroup(optionDlg.m_tab5_dlg.m_hotkey_group);
    theApp.m_hot_key_setting_data = optionDlg.m_tab5_dlg.m_data;

    if (reload_sf2 || output_device_changed || player_core_changed)
    {
        CPlayer::GetInstance().ReIniPlayerCore(true);
        UpdatePlayPauseButton();
        OnSetTitle(0, 0);
    }
    if (gauss_blur_changed)
        CPlayer::GetInstance().AlbumCoverGaussBlur();

    UpdatePlayPauseButton();

    ThemeColorChanged();
    ApplyThemeColor();

    if (optionDlg.m_tab2_dlg.FontChanged())
    {
        theApp.m_font_set.lyric.SetFont(theApp.m_app_setting_data.lyric_font);
        FontInfo translate_font = theApp.m_app_setting_data.lyric_font;
        translate_font.size--;
        theApp.m_font_set.lyric_translate.SetFont(translate_font);
    }
    if (optionDlg.m_tab1_dlg.FontChanged())
    {
        CCortanaLyric::InitFont();
    }

	m_desktop_lyric.ApplySettings(theApp.m_lyric_setting_data.desktop_lyric_data);

    SaveConfig();		//将设置写入到ini文件
    theApp.SaveConfig();
    CPlayer::GetInstance().SaveConfig();
    DrawInfo(true);
}

void CMusicPlayerDlg::ApplyThemeColor()
{
    CColorConvert::ConvertColor(theApp.m_app_setting_data.theme_color);
    SetPlayListColor();
    m_cortana_lyric.SetUIColors();
    DrawInfo();
    if (m_miniModeDlg.m_hWnd != NULL)
    {
        m_miniModeDlg.SetPlayListColor();
    }
}

void CMusicPlayerDlg::ThemeColorChanged()
{
    if (!theApp.m_app_setting_data.theme_color_follow_system)
        return;
    COLORREF color{};
    color = CCommon::GetWindowsThemeColor();
    CColorConvert::ReduceLuminance(color);				//如果主题颜色过深，就将其降低一点亮度
    if (theApp.m_app_setting_data.theme_color.original_color != color && color != RGB(255, 255, 255))	//当前主题色变了的时候重新设置主题色，但是确保获取到的颜色不是纯白色
    {
        theApp.m_app_setting_data.theme_color.original_color = color;
        ApplyThemeColor();
    }
}

void CMusicPlayerDlg::SetMenuState(CMenu * pMenu)
{
    //设置循环模式菜单的单选标记
    RepeatMode repeat_mode{ CPlayer::GetInstance().GetRepeatMode() };
    switch (repeat_mode)
    {
    case RM_PLAY_ORDER:
        pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_PLAY_ORDER, MF_BYCOMMAND | MF_CHECKED);
        break;
    case RM_PLAY_SHUFFLE:
        pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_PLAY_SHUFFLE, MF_BYCOMMAND | MF_CHECKED);
        break;
    case RM_LOOP_PLAYLIST:
        pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_LOOP_PLAYLIST, MF_BYCOMMAND | MF_CHECKED);
        break;
    case RM_LOOP_TRACK:
        pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_LOOP_TRACK, MF_BYCOMMAND | MF_CHECKED);
        break;
    default:
        break;
    }

    //弹出右键菜单时，如果没有选中播放列表中的项目，则禁用右键菜单中“播放”、“从列表中删除”、“属性”、“从磁盘删除”项目。
    bool selete_valid = m_item_selected >= 0 && m_item_selected < CPlayer::GetInstance().GetSongNum();
    bool from_playlist{ CPlayer::GetInstance().IsPlaylistMode() };
    pMenu->EnableMenuItem(ID_PLAY_ITEM, MF_BYCOMMAND | (selete_valid ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_REMOVE_FROM_PLAYLIST, MF_BYCOMMAND | (selete_valid && from_playlist ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_ITEM_PROPERTY, MF_BYCOMMAND | (selete_valid ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_DELETE_FROM_DISK, MF_BYCOMMAND | (selete_valid ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_EXPLORE_ONLINE, MF_BYCOMMAND | (selete_valid ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_COPY_FILE_TO, MF_BYCOMMAND | (selete_valid ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_MOVE_FILE_TO, MF_BYCOMMAND | (selete_valid ? MF_ENABLED : MF_GRAYED));

    pMenu->EnableMenuItem(ID_PLAYLIST_ADD_FILE, MF_BYCOMMAND | (from_playlist ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_PLAYLIST_ADD_FOLDER, MF_BYCOMMAND | (from_playlist ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_PLAYLIST_ADD_URL, MF_BYCOMMAND | (from_playlist ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_EMPTY_PLAYLIST, MF_BYCOMMAND | (from_playlist ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_REMOVE_SAME_SONGS, MF_BYCOMMAND | (from_playlist ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_REMOVE_INVALID_ITEMS, MF_BYCOMMAND | (from_playlist ? MF_ENABLED : MF_GRAYED));

    bool move_enable = from_playlist && !m_searched && selete_valid;
    pMenu->EnableMenuItem(ID_MOVE_PLAYLIST_ITEM_UP, MF_BYCOMMAND | (move_enable ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_MOVE_PLAYLIST_ITEM_DOWN, MF_BYCOMMAND | (move_enable ? MF_ENABLED : MF_GRAYED));

    //设置“添加到播放列表”子菜单项的可用状态
    bool use_default_playlist{ CPlayer::GetInstance().GetRecentPlaylist().m_cur_playlist_type == PT_DEFAULT };
    pMenu->EnableMenuItem(ID_ADD_TO_DEFAULT_PLAYLIST, MF_BYCOMMAND | (!(from_playlist && use_default_playlist) && selete_valid ? MF_ENABLED : MF_GRAYED));
    bool use_faourite_playlist{ CPlayer::GetInstance().GetRecentPlaylist().m_cur_playlist_type == PT_FAVOURITE };
    pMenu->EnableMenuItem(ID_ADD_TO_MY_FAVOURITE, MF_BYCOMMAND | (!(from_playlist && use_faourite_playlist) && selete_valid ? MF_ENABLED : MF_GRAYED));
    wstring current_playlist{ CPlayer::GetInstance().GetCurrentFolderOrPlaylistName() };
    for (UINT id = ID_ADD_TO_MY_FAVOURITE + 1; id < ID_ADD_TO_MY_FAVOURITE + ADD_TO_PLAYLIST_MAX_SIZE; id++)
    {
        CString menu_string;
        pMenu->GetMenuString(id, menu_string, 0);
        pMenu->EnableMenuItem(id, MF_BYCOMMAND | (selete_valid && current_playlist != menu_string.GetString() ? MF_ENABLED : MF_GRAYED));
    }
    pMenu->EnableMenuItem(ID_ADD_TO_NEW_PLAYLIST, MF_BYCOMMAND | (selete_valid ? MF_ENABLED : MF_GRAYED));

    //打开菜单时，如果播放列表中没有歌曲，则禁用主菜单和右键菜单中的“打开文件位置”项目
    if (CPlayer::GetInstance().GetSongNum() == 0)
    {
        pMenu->EnableMenuItem(ID_EXPLORE_PATH, MF_BYCOMMAND | MF_GRAYED);
        pMenu->EnableMenuItem(ID_EXPLORE_TRACK, MF_BYCOMMAND | MF_GRAYED);
        pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | MF_GRAYED);
        pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | MF_GRAYED);
        pMenu->EnableMenuItem(ID_DOWNLOAD_LYRIC, MF_BYCOMMAND | MF_GRAYED);
        pMenu->EnableMenuItem(ID_LYRIC_BATCH_DOWNLOAD, MF_BYCOMMAND | MF_GRAYED);
    }
    else
    {
        pMenu->EnableMenuItem(ID_EXPLORE_PATH, MF_BYCOMMAND | MF_ENABLED);
        pMenu->EnableMenuItem(ID_EXPLORE_TRACK, MF_BYCOMMAND | MF_ENABLED);
        pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | MF_ENABLED);
        pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | MF_ENABLED);
        pMenu->EnableMenuItem(ID_DOWNLOAD_LYRIC, MF_BYCOMMAND | MF_ENABLED);
        pMenu->EnableMenuItem(ID_LYRIC_BATCH_DOWNLOAD, MF_BYCOMMAND | MF_ENABLED);
    }

    //设置“视图”菜单下的复选标记
    pMenu->CheckMenuItem(ID_SHOW_PLAYLIST, MF_BYCOMMAND | (theApp.m_ui_data.show_playlist ? MF_CHECKED : MF_UNCHECKED));
    pMenu->CheckMenuItem(ID_SHOW_MENU_BAR, MF_BYCOMMAND | (theApp.m_ui_data.show_menu_bar ? MF_CHECKED : MF_UNCHECKED));
    pMenu->CheckMenuItem(ID_FULL_SCREEN, MF_BYCOMMAND | (theApp.m_ui_data.full_screen ? MF_CHECKED : MF_UNCHECKED));
    pMenu->CheckMenuItem(ID_DARK_MODE, MF_BYCOMMAND | (theApp.m_app_setting_data.dark_mode ? MF_CHECKED : MF_UNCHECKED));
    pMenu->CheckMenuItem(ID_ALWAYS_ON_TOP, MF_BYCOMMAND | (theApp.m_nc_setting_data.always_on_top ? MF_CHECKED : MF_UNCHECKED));

    pMenu->EnableMenuItem(ID_SHOW_MENU_BAR, MF_BYCOMMAND | (theApp.m_ui_data.full_screen ? MF_GRAYED : MF_ENABLED));        //全屏时禁止显示/关闭菜单栏

    pMenu->CheckMenuItem(ID_FLOAT_PLAYLIST, MF_BYCOMMAND | (theApp.m_nc_setting_data.float_playlist ? MF_CHECKED : MF_UNCHECKED));

    //设置播放列表菜单中排序方式的单选标记
    if(!CPlayer::GetInstance().IsPlaylistMode())
    {
        switch (CPlayer::GetInstance().m_sort_mode)
        {
        case SM_FILE:
            pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_FILE, MF_BYCOMMAND | MF_CHECKED);
            break;
        case SM_TITLE:
            pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_TITLE, MF_BYCOMMAND | MF_CHECKED);
            break;
        case SM_ARTIST:
            pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_ARTIST, MF_BYCOMMAND | MF_CHECKED);
            break;
        case SM_ALBUM:
            pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_ALBUM, MF_BYCOMMAND | MF_CHECKED);
            break;
        case SM_TRACK:
            pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_TRACK, MF_BYCOMMAND | MF_CHECKED);
            break;
        }
    }
    else
    {
        pMenu->CheckMenuItem(ID_SORT_BY_FILE, MF_UNCHECKED);
        pMenu->CheckMenuItem(ID_SORT_BY_TITLE, MF_UNCHECKED);
        pMenu->CheckMenuItem(ID_SORT_BY_ARTIST, MF_UNCHECKED);
        pMenu->CheckMenuItem(ID_SORT_BY_ALBUM, MF_UNCHECKED);
        pMenu->CheckMenuItem(ID_SORT_BY_TRACK, MF_UNCHECKED);
    }

    //设置播放列表菜单中“播放列表显示样式”的单选标记
    switch (theApp.m_ui_data.display_format)
    {
    case DF_FILE_NAME:
        pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_FILE_NAME, MF_BYCOMMAND | MF_CHECKED);
        break;
    case DF_TITLE:
        pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_TITLE, MF_BYCOMMAND | MF_CHECKED);
        break;
    case DF_ARTIST_TITLE:
        pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_ARTIST_TITLE, MF_BYCOMMAND | MF_CHECKED);
        break;
    case DF_TITLE_ARTIST:
        pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_TITLE_ARTIST, MF_BYCOMMAND | MF_CHECKED);
        break;
    }

    if(theApp.m_nc_setting_data.playlist_btn_for_float_playlist)
        pMenu->CheckMenuRadioItem(ID_DOCKED_PLAYLIST, ID_FLOATED_PLAYLIST, ID_FLOATED_PLAYLIST, MF_BYCOMMAND | MF_CHECKED);
    else
        pMenu->CheckMenuRadioItem(ID_DOCKED_PLAYLIST, ID_FLOATED_PLAYLIST, ID_DOCKED_PLAYLIST, MF_BYCOMMAND | MF_CHECKED);

    //设置播放列表右键菜单的默认菜单项
    pMenu->SetDefaultItem(ID_PLAY_ITEM);

    //根据歌词是否存在设置启用或禁用菜单项
    bool midi_lyric{ CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !CPlayer::GetInstance().MidiNoLyric() };
    bool lyric_disable{ midi_lyric || CPlayer::GetInstance().m_Lyrics.IsEmpty() };
    bool no_lyric{ CPlayer::GetInstance().m_Lyrics.IsEmpty() && CPlayer::GetInstance().MidiNoLyric() };
    //pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | (!CPlayer::GetInstance().m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_COPY_CURRENT_LYRIC, MF_BYCOMMAND | (!no_lyric ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_COPY_ALL_LYRIC, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
    //pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | (!CPlayer::GetInstance().m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_LYRIC_FORWARD, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_LYRIC_DELAY, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_SAVE_MODIFIED_LYRIC, MF_BYCOMMAND | ((!lyric_disable && CPlayer::GetInstance().m_Lyrics.IsModified()) ? MF_ENABLED : MF_GRAYED));
    if (midi_lyric)
        pMenu->EnableMenuItem(ID_DELETE_LYRIC, MF_BYCOMMAND | MF_GRAYED);
    else
        pMenu->EnableMenuItem(ID_DELETE_LYRIC, MF_BYCOMMAND | (CCommon::FileExist(CPlayer::GetInstance().m_Lyrics.GetPathName()) ? MF_ENABLED : MF_GRAYED));		//当歌词文件存在时启用“删除歌词”菜单项
    pMenu->EnableMenuItem(ID_BROWSE_LYRIC, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_TRANSLATE_TO_SIMPLIFIED_CHINESE, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_TRANSLATE_TO_TRANDITIONAL_CHINESE, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | (!midi_lyric ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | (!midi_lyric ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_DOWNLOAD_LYRIC, MF_BYCOMMAND | (!midi_lyric ? MF_ENABLED : MF_GRAYED));

    pMenu->EnableMenuItem(ID_ALBUM_COVER_SAVE_AS, MF_BYCOMMAND | (CPlayer::GetInstance().AlbumCoverExist() ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_DOWNLOAD_ALBUM_COVER, MF_BYCOMMAND | (!CPlayer::GetInstance().IsOsuFile() && !CPlayer::GetInstance().IsInnerCover() ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_DELETE_ALBUM_COVER, MF_BYCOMMAND | ((!CPlayer::GetInstance().IsOsuFile() && !CPlayer::GetInstance().IsInnerCover() && CPlayer::GetInstance().AlbumCoverExist()) ? MF_ENABLED : MF_GRAYED));

    //正在执行格式转换时禁用“格式转换”菜单项
    pMenu->EnableMenuItem(ID_FORMAT_CONVERT, MF_BYCOMMAND | (theApp.m_format_convert_dialog_exit ? MF_ENABLED : MF_GRAYED));
    pMenu->EnableMenuItem(ID_FORMAT_CONVERT1, MF_BYCOMMAND | (theApp.m_format_convert_dialog_exit ? MF_ENABLED : MF_GRAYED));

    pMenu->CheckMenuItem(ID_LOCK_DESKTOP_LRYIC, MF_BYCOMMAND | (theApp.m_lyric_setting_data.desktop_lyric_data.lock_desktop_lyric ? MF_CHECKED : MF_UNCHECKED));
    pMenu->CheckMenuItem(ID_LYRIC_DISPLAYED_DOUBLE_LINE, MF_BYCOMMAND | (theApp.m_lyric_setting_data.desktop_lyric_data.lyric_double_line ? MF_CHECKED : MF_UNCHECKED));
    pMenu->CheckMenuItem(ID_LYRIC_BACKGROUND_PENETRATE, MF_BYCOMMAND | (theApp.m_lyric_setting_data.desktop_lyric_data.lyric_background_penetrate ? MF_CHECKED : MF_UNCHECKED));
}

void CMusicPlayerDlg::ShowFloatPlaylist()
{
    CCommon::DeleteModelessDialog(m_pFloatPlaylistDlg);
    m_pFloatPlaylistDlg = new CFloatPlaylistDlg(m_item_selected, m_items_selected);
    m_pFloatPlaylistDlg->Create(IDD_MUSICPLAYER2_DIALOG, GetDesktopWindow());
    m_pFloatPlaylistDlg->ShowWindow(SW_SHOW);
    if(m_float_playlist_pos.x != 0 && m_float_playlist_pos.y != 0)
        m_pFloatPlaylistDlg->SetWindowPos(nullptr, m_float_playlist_pos.x, m_float_playlist_pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    theApp.m_ui_data.show_playlist = false;
    SetPlaylistVisible();
    DrawInfo(true);
}

void CMusicPlayerDlg::HideFloatPlaylist()
{
    OnFloatPlaylistClosed(0, 0);
    CCommon::DeleteModelessDialog(m_pFloatPlaylistDlg);
}

void CMusicPlayerDlg::GetPlaylistItemSelected()
{
    if (!m_searched)
    {
        m_item_selected = m_playlist_list.GetCurSel();	//获取鼠标选中的项目
        m_playlist_list.GetItemSelected(m_items_selected);		//获取多个选中的项目
    }
    else
    {
        CString str;
        str = m_playlist_list.GetItemText(m_playlist_list.GetCurSel(), 0);
        m_item_selected = _ttoi(str) - 1;
        m_playlist_list.GetItemSelectedSearched(m_items_selected);
    }

}

void CMusicPlayerDlg::IniPlaylistPopupMenu()
{
    theApp.m_menu_set.m_list_popup_menu.DestroyMenu();
    theApp.m_menu_set.m_list_popup_menu.LoadMenu(IDR_POPUP_MENU);
    CMenu* add_to_menu = theApp.m_menu_set.m_list_popup_menu.GetSubMenu(0)->GetSubMenu(10);
    ASSERT(add_to_menu != nullptr);
    if (add_to_menu != nullptr)
    {
        auto& recent_playlist{ CPlayer::GetInstance().GetRecentPlaylist().m_recent_playlists };
        for (size_t i{}; i < recent_playlist.size() && i < ADD_TO_PLAYLIST_MAX_SIZE; i++)
        {
            CFilePathHelper playlist_path{ recent_playlist[i].path };
            add_to_menu->AppendMenu(MF_STRING | MF_ENABLED, ID_ADD_TO_MY_FAVOURITE + i + 1, playlist_path.GetFileNameWithoutExtension().c_str());
        }
    }
}

void CMusicPlayerDlg::SetPlaylistDragEnable()
{
    bool enable = CPlayer::GetInstance().IsPlaylistMode() && !m_searched;   //处于播放列表模式且不处理搜索状态时才允许拖动排序
    m_playlist_list.SetDragEnable(enable);

    if (m_pFloatPlaylistDlg->GetSafeHwnd() != NULL)
        m_pFloatPlaylistDlg->SetDragEnable();
    if(m_miniModeDlg.GetSafeHwnd()!=NULL)
        m_miniModeDlg.SetDragEnable();
}

BOOL CMusicPlayerDlg::OnInitDialog()
{
    CMainDialogBase::OnInitDialog();

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码

    //载入图标资源
    theApp.LoadIconResource();

    //初始化字体
    theApp.m_font_set.Init();

    theApp.InitMenuResourse();

    //载入设置
    LoadConfig();

    //只有Windows Vista以上的系统才能跟随系统主题色
#ifdef COMPILE_IN_WIN_XP
    theApp.m_app_setting_data.theme_color_follow_system = false;
//#else
//	if (!CWinVersionHelper::IsWindows8OrLater())
//		theApp.m_app_setting_data.theme_color_follow_system = false;
#endif

    //只有Win10以上的系统才能在Cortana搜索框中显示歌词
    if (!CWinVersionHelper::IsWindows10OrLater())
        theApp.m_lyric_setting_data.cortana_info_enable = false;
    m_cortana_lyric.SetEnable(CWinVersionHelper::IsWindows10OrLater());

    //设置窗口不透明度
    SetTransparency();
    SetDesptopLyricTransparency();

    //初始化窗口大小
    //rect.right = m_window_width;
    //rect.bottom = m_window_height;
    if (m_window_height != -1 && m_window_width != -1)
    {
        //MoveWindow(rect);
        SetWindowPos(nullptr, 0, 0, m_window_width, m_window_height, SWP_NOZORDER | SWP_NOMOVE);
    }

    //初始化提示信息
    m_Mytip.Create(this, TTS_ALWAYSTIP);
    m_Mytip.SetMaxTipWidth(theApp.DPI(400));
    m_Mytip.AddTool(GetDlgItem(ID_SET_PATH), CCommon::LoadText(IDS_OPEN_MEDIA_LIB, _T(" (Ctrl+T)")));
    //m_Mytip.AddTool(GetDlgItem(IDC_VOLUME_DOWN), _T("减小音量"));
    //m_Mytip.AddTool(GetDlgItem(IDC_VOLUME_UP), _T("增大音量"));
    //m_Mytip.AddTool(&m_time_static, CCommon::LoadText(IDS_PLAY_TIME));
    m_Mytip.AddTool(&m_clear_search_button, CCommon::LoadText(IDS_CLEAR_SEARCH_RESULT));
    m_Mytip.AddTool(&m_search_edit, CCommon::LoadText(IDS_INPUT_KEY_WORD));
    m_ui.SetToolTip(&m_Mytip);
    m_ui2.SetToolTip(&m_Mytip);

    //为显示播放时间的static控件设置SS_NOTIFY属性，以启用鼠标提示
    //DWORD dwStyle = m_time_static.GetStyle();
    //::SetWindowLong(m_time_static.GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

    SetMenubarVisible();


    m_search_edit.SetCueBanner(CCommon::LoadText(IDS_SEARCH_HERE), TRUE);

    //CoInitialize(0);	//初始化COM组件，用于支持任务栏显示进度和缩略图按钮
#ifndef COMPILE_IN_WIN_XP
    //if (CWinVersionHelper::IsWindows7OrLater())
    //	CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pTaskbar));	//创建ITaskbarList3的实例

    //初始化任务栏缩略图中的按钮
    THUMBBUTTONMASK dwMask = THB_ICON | THB_TOOLTIP | THB_FLAGS;
    //上一曲按钮
    m_thumbButton[0].dwMask = dwMask;
    m_thumbButton[0].iId = IDT_PREVIOUS;
    m_thumbButton[0].hIcon = theApp.m_icon_set.previous.GetIcon();
    wcscpy_s(m_thumbButton[0].szTip, CCommon::LoadText(IDS_PREVIOUS));
    m_thumbButton[0].dwFlags = THBF_ENABLED;
    //播放/暂停按钮
    m_thumbButton[1].dwMask = dwMask;
    m_thumbButton[1].iId = IDT_PLAY_PAUSE;
    m_thumbButton[1].hIcon = theApp.m_icon_set.play.GetIcon();
    wcscpy_s(m_thumbButton[1].szTip, CCommon::LoadText(IDS_PLAY));
    m_thumbButton[1].dwFlags = THBF_ENABLED;
    //下一曲按钮
    m_thumbButton[2].dwMask = dwMask;
    m_thumbButton[2].iId = IDT_NEXT;
    m_thumbButton[2].hIcon = theApp.m_icon_set.next.GetIcon();
    wcscpy_s(m_thumbButton[2].szTip, CCommon::LoadText(IDS_NEXT));
    m_thumbButton[2].dwFlags = THBF_ENABLED;
#endif

    //注册全局热键
    if(theApp.m_hot_key_setting_data.hot_key_enable)
        theApp.m_hot_key.RegisterAllHotKey();

    //设置界面的颜色
    CColorConvert::ConvertColor(theApp.m_app_setting_data.theme_color);

    //初始化查找对话框中的数据
    m_findDlg.LoadConfig();

    //获取Cortana歌词
    m_cortana_lyric.Init();

	//初始化桌面歌词
	m_desktop_lyric.Create();
	m_desktop_lyric.ApplySettings(theApp.m_lyric_setting_data.desktop_lyric_data);
    if (m_desktop_lyric_pos.x != -1 && m_desktop_lyric_pos.y != -1)
    {
        CRect rcLyric;
        ::GetWindowRect(m_desktop_lyric.GetLyricWnd(), rcLyric);
        CRect rcWork;
        SystemParametersInfo(SPI_GETWORKAREA, NULL, rcWork, NULL);
        if (m_desktop_lyric_pos.x < rcWork.left - rcLyric.Width() / 2)
            m_desktop_lyric_pos.x = rcWork.left - rcLyric.Width() / 2;
        if (m_desktop_lyric_pos.x > rcWork.right - rcLyric.Width() / 2)
            m_desktop_lyric_pos.x = rcWork.right - rcLyric.Width() / 2;
        if (m_desktop_lyric_pos.y < rcWork.top - rcLyric.Height() / 2)
            m_desktop_lyric_pos.y = rcWork.top - rcLyric.Height() / 2;
        if (m_desktop_lyric_pos.y > rcWork.bottom - rcLyric.Height() / 2)
            m_desktop_lyric_pos.y = rcWork.bottom - rcLyric.Height() / 2;
        ::SetWindowPos(m_desktop_lyric.GetLyricWnd(), nullptr, m_desktop_lyric_pos.x, m_desktop_lyric_pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
    if (m_desktop_lyric_size.cx > 0 && m_desktop_lyric_size.cy > 0)
    {
        if (m_desktop_lyric_size.cx < theApp.DPI(400))
            m_desktop_lyric_size.cx = theApp.DPI(400);
        if (m_desktop_lyric_size.cy < theApp.DPI(100))
            m_desktop_lyric_size.cy = theApp.DPI(100);
        ::SetWindowPos(m_desktop_lyric.GetLyricWnd(), nullptr, 0, 0, m_desktop_lyric_size.cx, m_desktop_lyric_size.cy, SWP_NOMOVE | SWP_NOZORDER);
    }

    //初始化绘图的类
    m_pDC = GetDC();
    //m_draw.Create(m_pDC, this);
    m_ui.Init(m_pDC);
    m_ui2.Init(m_pDC);
    //m_pUI = &m_ui2;

    //初始化歌词字体
    theApp.m_font_set.lyric.SetFont(theApp.m_app_setting_data.lyric_font);
    FontInfo translate_font = theApp.m_app_setting_data.lyric_font;
    translate_font.size--;
    theApp.m_font_set.lyric_translate.SetFont(translate_font);

    //载入默认背景图片（用于没有专辑封面时显示）
    theApp.m_ui_data.default_background.Load((theApp.m_local_dir + L"default_background.jpg").c_str());

    m_notify_icon.Init(m_hIcon);
    m_notify_icon.AddNotifyIcon();

    //设置定时器
    SetTimer(TIMER_ID, TIMER_ELAPSE, NULL);
    SetTimer(TIMER_1_SEC, 1000, NULL);

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMusicPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    DWORD cmd = nID & 0xFFF0;
    /*if (cmd == IDM_ABOUTBOX)
    {
    	CAboutDlg dlgAbout;
    	dlgAbout.DoModal();
    }
    else */if (cmd == IDM_MINIMODE)
    {
        OnMiniMode();
    }
    else
    {
        CMainDialogBase::OnSysCommand(nID, lParam);
    }

    if (nID >= 0x8000)
    {
        SendMessage(WM_COMMAND, nID);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMusicPlayerDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CMainDialogBase::OnPaint();
    }
    //DrawInfo();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMusicPlayerDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CMusicPlayerDlg::OnSize(UINT nType, int cx, int cy)
{
    CMainDialogBase::OnSize(nType, cx, cy);
    if (nType != SIZE_MINIMIZED && m_pUI != nullptr)
    {
        if (m_pDC != NULL)
        {
            DrawInfo(true);
            if ((cx < m_ui.WidthThreshold()) != theApp.m_ui_data.narrow_mode)	//如果在窄界面模式和普通模式之间进行了切换，则重绘客户区
            {
                Invalidate(FALSE);
                //m_time_static.Invalidate(FALSE);
            }
            m_pUI->OnSizeRedraw(cx, cy);
        }
        theApp.m_ui_data.client_width = cx;
        theApp.m_ui_data.client_height = cy;
        if (m_ui.WidthThreshold() != 0)
        {
            theApp.m_ui_data.narrow_mode = (cx < m_ui.WidthThreshold());
            //if (!theApp.m_ui_data.show_playlist)
            //	theApp.m_ui_data.narrow_mode = false;
        }
        if (m_playlist_list.m_hWnd)
        {
            SetPlaylistSize(cx, cy);
        }

        if (nType != SIZE_MAXIMIZED && !theApp.m_ui_data.full_screen)
        {
            CRect rect;
            GetWindowRect(&rect);
            m_window_width = rect.Width();
            m_window_height = rect.Height();
        }

        //窗口大小变化时更新界面鼠标提示的位置
        static UINT last_type;
        if (last_type != nType)
        {
            if (m_pUI != nullptr)
                DrawInfo();
        }
        last_type = nType;
        m_pUI->UpdateToolTipPosition();
    }

    SetThumbnailClipArea();


    // TODO: 在此处添加消息处理程序代码
}


void CMusicPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    //响应主定时器
    if(nIDEvent == TIMER_ID)
    {
        if (m_first_start)
        {
            //此if语句只在定时器第1次触发时才执行
            m_first_start = false;
#ifndef COMPILE_IN_WIN_XP
            if (IsTaskbarListEnable())
            {
                //设置任务栏缩略图窗口按钮
                m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);
            }
#endif
            CRect rect;
            GetClientRect(rect);
            theApp.m_ui_data.client_width = rect.Width();
            theApp.m_ui_data.client_height = rect.Height();
            SetPlaylistSize(rect.Width(), rect.Height());		//调整播放列表的大小和位置
            m_path_static.Invalidate();
            //SetPorgressBarSize(rect.Width(), rect.Height());		//调整进度条在窗口中的大小和位置
            SetPlaylistVisible();

            if (m_cmdLine.empty())		//没有有通过命令行打开文件
            {
                CPlayer::GetInstance().Create();
            }
            else if (m_cmdLine.find(L"RestartByRestartManager") != wstring::npos)		//如果命令行参数中有RestartByRestartManager，则忽略命令行参数
            {
                CPlayer::GetInstance().Create();
                ////将命令行参数写入日志文件
                //wchar_t buff[256];
                //swprintf_s(buff, L"程序已被Windows的RestartManager重启，重启参数：%s", m_cmdLine.c_str());
                //theApp.WriteErrorLog(wstring{ buff });
            }
            else		//从命令行参数获取要打开的文件
            {
                vector<wstring> files;
                wstring path = CCommon::DisposeCmdLineFiles(m_cmdLine, files);
                if (!path.empty())
                    CPlayer::GetInstance().Create(path);
                else
                    CPlayer::GetInstance().Create(files);
                //MessageBox(m_cmdLine.c_str(), NULL, MB_ICONINFORMATION);
            }
            DrawInfo();
            SetThumbnailClipArea();

            //注：不应该在这里打开或播放歌曲，应该在播放列表初始化完毕时执行。
            //CPlayer::GetInstance().MusicControl(Command::OPEN);
            //CPlayer::GetInstance().MusicControl(Command::SEEK);
            //CPlayer::GetInstance().GetPlayerCoreError();
            //SetPorgressBarSize(rect.Width(), rect.Height());		//重新调整进度条在窗口中的大小和位置（需要根据歌曲的时长调整显示时间控件的宽度）
            //ShowTime();
            //m_progress_bar.SetSongLength(CPlayer::GetInstance().GetSongLength());

            //if(!m_cmdLine.empty())
            //	CPlayer::GetInstance().MusicControl(Command::PLAY);	//如果文件是通过命令行打开的，则打开后直接播放

            UpdatePlayPauseButton();
            //SetForegroundWindow();
            //ShowPlayList();

            ThemeColorChanged();

            if (theApp.m_nc_setting_data.float_playlist)
                ShowFloatPlaylist();

            IniPlaylistPopupMenu();

            //提示用户是否创建桌面快捷方式
            CreateDesktopShortcut();

            SetAlwaysOnTop();
        }

        m_timer_count++;

        UpdateTaskBarProgress();
        //UpdateProgress();

        //CPlayer::GetInstance().GetPlayerCoreError();
        if (m_miniModeDlg.m_hWnd == NULL && (CPlayer::GetInstance().IsPlaying() || GetActiveWindow() == this))		//进入迷你模式时不刷新，不在播放且窗口处于后台时不刷新
            DrawInfo();			//绘制界面上的信息（如果显示了迷你模式，则不绘制界面信息）
        CPlayer::GetInstance().CalculateSpectralData();
        if (CPlayer::GetInstance().IsPlaying())
        {
            CPlayer::GetInstance().GetPlayerCoreCurrentPosition();
        }

        //在Cortana搜索框里显示歌词
        if (theApp.m_lyric_setting_data.cortana_info_enable)
        {
            if (theApp.m_lyric_setting_data.cortana_lyric_keep_display || CPlayer::GetInstance().IsPlaying())
                m_cortana_lyric.DrawInfo();
        }

		//显示桌面歌词
		bool desktop_lyric_visible = theApp.m_lyric_setting_data.show_desktop_lyric;
		if (CPlayer::GetInstance().m_Lyrics.IsEmpty() && theApp.m_lyric_setting_data.desktop_lyric_data.hide_lyric_window_without_lyric)
			desktop_lyric_visible = false;
        if(!CPlayer::GetInstance().IsPlaying() && theApp.m_lyric_setting_data.desktop_lyric_data.hide_lyric_window_when_paused)
            desktop_lyric_visible = false;
		m_desktop_lyric.SetLyricWindowVisible(desktop_lyric_visible);
		if (theApp.m_lyric_setting_data.show_desktop_lyric)
		{
			m_desktop_lyric.ShowLyric();
		}

        //if (CPlayer::GetInstance().SongIsOver() && (!theApp.m_lyric_setting_data.stop_when_error || !CPlayer::GetInstance().IsError()))	//当前曲目播放完毕且没有出现错误时才播放下一曲
        if ((CPlayer::GetInstance().SongIsOver() || (!theApp.m_play_setting_data.stop_when_error && CPlayer::GetInstance().IsError())) && m_play_error_cnt <= CPlayer::GetInstance().GetSongNum())	//当前曲目播放完毕且没有出现错误时才播放下一曲
        {
            if (CPlayer::GetInstance().IsError())
                m_play_error_cnt++;
            else
                m_play_error_cnt = 0;
            //当前正在编辑歌词，或顺序播放模式下列表中的歌曲播放完毕时（PlayTrack函数会返回false），播放完当前歌曲就停止播放
            if ((m_pLyricEdit != nullptr && m_pLyricEdit->m_dlg_exist) || !CPlayer::GetInstance().PlayTrack(NEXT))
            {
                CPlayer::GetInstance().MusicControl(Command::STOP);		//停止播放
                //ShowTime();
                if (theApp.m_lyric_setting_data.cortana_info_enable)
                    m_cortana_lyric.ResetCortanaText();
            }
            SwitchTrack();
            UpdatePlayPauseButton();
        }
        if (CPlayer::GetInstance().IsPlaying() && (theApp.m_play_setting_data.stop_when_error && CPlayer::GetInstance().IsError()))
        {
            CPlayer::GetInstance().MusicControl(Command::PAUSE);
            UpdatePlayPauseButton();
        }

        //if (m_timer_count % 10 == 0)
        //	m_cortana_lyric.CheckDarkMode();

        if (CWinVersionHelper::IsWindowsVista())
        {
            if (m_timer_count % 15 == 14)
                ThemeColorChanged();
        }

        if (m_timer_count % 600 == 599)
        {
            //CPlayer::GetInstance().EmplaceCurrentPathToRecent();
            //CPlayer::GetInstance().SaveRecentPath();
            if(theApp.IsSongDataModified())				//在歌曲信息被修改过的情况下，每隔一定的时间保存一次
                theApp.SaveSongData();
        }
    }

    //响应1秒定时器
    else if (nIDEvent == TIMER_1_SEC)
    {
        if (CPlayer::GetInstance().IsPlaying())
        {
            CPlayer::GetInstance().AddListenTime(1);
        }

    }

    else if (nIDEvent == DELAY_TIMER_ID)
    {
        KillTimer(DELAY_TIMER_ID);
        m_no_lbtnup = false;
    }

    CMainDialogBase::OnTimer(nIDEvent);
}


void CMusicPlayerDlg::OnPlayPause()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().MusicControl(Command::PLAY_PAUSE);
    if (!CPlayer::GetInstance().IsPlaying())
        DrawInfo();
    UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnStop()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().MusicControl(Command::STOP);
    UpdatePlayPauseButton();
    //ShowTime();
}


void CMusicPlayerDlg::OnPrevious()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().PlayTrack(PREVIOUS);
    SwitchTrack();
    UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnNext()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().PlayTrack(NEXT);
    SwitchTrack();
    UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnRew()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().MusicControl(Command::REW);
    UpdateTaskBarProgress();
    //ShowTime();
}


void CMusicPlayerDlg::OnFF()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().MusicControl(Command::FF);
    UpdateTaskBarProgress();
    //ShowTime();
}


void CMusicPlayerDlg::OnSetPath()
{
    static bool dialog_exist{ false };

    if (!dialog_exist)		//确保对话框已经存在时不再弹出
    {
        dialog_exist = true;
        int cur_tab{ CPlayer::GetInstance().IsPlaylistMode() ? 1 : 0 };
        CMediaLibDlg media_lib_dlg{ cur_tab };
        media_lib_dlg.DoModal();
        dialog_exist = false;
        if (media_lib_dlg.m_playlist_dlg.IsPlaylistModified())
            IniPlaylistPopupMenu();
    }
}


afx_msg LRESULT CMusicPlayerDlg::OnPathSelected(WPARAM wParam, LPARAM lParam)
{
    CSetPathDlg* pPathDlg = (CSetPathDlg*)wParam;
    if (pPathDlg != nullptr)
    {
        CPlayer::GetInstance().SetPath(pPathDlg->GetSelPath(), pPathDlg->GetTrack(), pPathDlg->GetPosition(), pPathDlg->GetSortMode());
        UpdatePlayPauseButton();
        //SetPorgressBarSize();
        //ShowTime();
        DrawInfo(true);
        m_findDlg.ClearFindResult();		//更换路径后清除查找结果
        CPlayer::GetInstance().SaveRecentPath();
        m_play_error_cnt = 0;
        SetTimer(DELAY_TIMER_ID, 500, NULL);        //在媒体库对话框中选择了一个文件夹播放后，500毫秒内不响应WM_LBUTTONUP消息
        m_no_lbtnup = true;
    }
    return 0;
}


void CMusicPlayerDlg::OnFind()
{
    // TODO: 在此添加命令处理程序代码
    if (m_findDlg.DoModal() == IDOK)
    {
        if (m_findDlg.GetFindCurrentPlaylist())
        {
            int selected_track{ m_findDlg.GetSelectedTrack() };
            if (selected_track >= 0)
            {
                CPlayer::GetInstance().PlayTrack(m_findDlg.GetSelectedTrack());
                SwitchTrack();
                UpdatePlayPauseButton();
            }
        }
        else
        {
            wstring selected_song_path{ m_findDlg.GetSelectedSongPath() };
            if (!CCommon::FileExist(selected_song_path))
            {
                //如果文件不存在，则弹出错误信息
                CString info;
                info = CCommon::LoadTextFormat(IDS_CONNOT_FIND_FILE, { selected_song_path });
                MessageBox(info, NULL, MB_ICONWARNING);
                return;
            }
            if (!selected_song_path.empty())
            {
                CPlayer::GetInstance().OpenAFile(selected_song_path);
                SwitchTrack();
                UpdatePlayPauseButton();
            }
        }
    }
}


void CMusicPlayerDlg::OnExplorePath()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().ExplorePath();
}


BOOL CMusicPlayerDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if (pMsg->message == WM_KEYDOWN && pMsg->hwnd != m_search_edit.GetSafeHwnd())
    {
        //按下Ctrl键时
        if (GetKeyState(VK_CONTROL) & 0x80)
        {
            if (pMsg->wParam == 'O')		//设置按Ctr+O打开文件
            {
                OnFileOpen();
                return TRUE;
            }
            if (pMsg->wParam == 'D')		//设置按Ctr+D打开文件夹
            {
                OnFileOpenFolder();
                return TRUE;
            }
            if (pMsg->wParam == 'B')		//设置按Ctr+B浏览文件
            {
                OnExplorePath();
                return TRUE;
            }
            if (pMsg->wParam == 'N')		//设置按Ctr+N打开曲目信息
            {
                OnSongInfo();
                return TRUE;
            }
            if (pMsg->wParam == 'F')		//按Ctr+F键查找文件
            {
                OnFind();
                return TRUE;
            }
            if (pMsg->wParam == 'T')		//按Ctr+T键设置路径
            {
                OnSetPath();
                return TRUE;
            }
            if (pMsg->wParam == 'I')		//设置按Ctr+I打开设置
            {
                OnOptionSettings();
                return TRUE;
            }
            //if (pMsg->wParam == 'X')		//设置按Ctr+X退出
            //{
            //	OnMenuExit();
            //	return TRUE;
            //}
            if (pMsg->wParam == 'R')		//设置按Ctr+R打开录音机
            {
                OnRecorder();
                return TRUE;
            }
            if (pMsg->wParam == 'M')		//设置按Ctr+M进入迷你模式
            {
                OnMiniMode();
                return TRUE;
            }
            if (pMsg->wParam == VK_PRIOR)		//设置按Ctr+PgUp歌词提前0.5秒
            {
                OnLyricForward();
                return TRUE;
            }
            if (pMsg->wParam == VK_NEXT)		//设置按Ctr+PgDn歌词延后0.5秒
            {
                OnLyricDelay();
                return TRUE;
            }
            if (pMsg->wParam == 'S')		//设置按Ctr+S保存歌词更改
            {
                if (!CPlayer::GetInstance().m_Lyrics.IsEmpty() && CPlayer::GetInstance().m_Lyrics.IsModified())
                    OnSaveModifiedLyric();
                return TRUE;
            }
            if (pMsg->wParam == 'E')		//设置按Ctr+S打开均衡器
            {
                OnEqualizer();
                return TRUE;
            }
            if (pMsg->wParam == 'L')		//设置按Ctr+L显示/隐藏播放列表
            {
                OnShowPlaylist();
                return TRUE;
            }
            if (pMsg->wParam == 'K')		//设置按Ctr+K显示浮动播放列表
            {
                OnFloatPlaylist();
                return TRUE;
            }
            if (pMsg->wParam == VK_UP)
            {
                OnMovePlaylistItemUp();
                return TRUE;
            }
            if (pMsg->wParam == VK_DOWN)
            {
                OnMovePlaylistItemDown();
                return TRUE;
            }
            if (GetKeyState(VK_SHIFT) & 0x8000)
            {
				//按下Ctrl + Shift键时
#ifdef _DEBUG
				if (pMsg->wParam == 'Z')
				{
                    //vector<wstring> files;
                    //files.push_back(L"D:\\Music\\纯音乐\\[.que] - Decide.mp3");
                    //files.push_back(L"D:\\Music\\小冰\\小冰 - 24小时之歌.mp3");
                    //CPlayer::GetInstance().OpenFiles(files, false);
                }
#endif
            }
        }
        else
        {
            if (pMsg->wParam == VK_SPACE || pMsg->wParam == 'P'/* || pMsg->wParam == VK_MEDIA_PLAY_PAUSE*/)		//按空格键/P键播放/暂停
            {
                OnPlayPause();
                return TRUE;
            }
            if (pMsg->wParam == 'S')	//按S键停止
            {
                OnStop();
                return TRUE;
            }
            if (pMsg->wParam == 'V')	//按V键上一曲
            {
                OnPrevious();
                return TRUE;
            }
            if (pMsg->wParam == 'N')	//按N键下一曲
            {
                OnNext();
                return TRUE;
            }
            if (pMsg->wParam == VK_LEFT)	//按左方向键下快退5秒
            {
                OnRew();
                return TRUE;
            }
            if (pMsg->wParam == VK_RIGHT)	//按右方向键下快进5秒
            {
                OnFF();
                return TRUE;
            }
            if (pMsg->wParam == VK_UP)	//按上方向键下音量加
            {
                OnVolumeUp();
                return TRUE;
            }
            if (pMsg->wParam == VK_DOWN)	//按下方向键音量减
            {
                OnVolumeDown();
                return TRUE;
            }
            if (pMsg->wParam == 'M')	//按M键设置循环模式
            {
                CPlayer::GetInstance().SetRepeatMode();
                m_ui.UpdateRepeatModeToolTip();
                m_ui2.UpdateRepeatModeToolTip();
                return TRUE;
            }
            if (pMsg->wParam == 'F')	//按F键快速查找
            {
                m_search_edit.SetFocus();
                return TRUE;
            }
            if (pMsg->wParam == VK_F11)	//按F11键进入或退出全屏模式
            {
                OnFullScreen();
                return TRUE;
            }
            if (pMsg->wParam == VK_ESCAPE)	//按ESC键退出全屏模式
            {
                if (theApp.m_ui_data.full_screen)
                {
                    OnFullScreen();
                    return TRUE;
                }
            }

            if (pMsg->wParam == VK_APPS)		//按菜单键弹出主菜单
            {
                SendMessage(WM_MAIN_MENU_POPEDUP, (WPARAM)&CPoint(0, 0));
                return TRUE;
            }
        }
    }

    if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))		//屏蔽按回车键和ESC键退出
    {
        return TRUE;
    }


    if (pMsg->message == WM_MOUSEMOVE)
        m_Mytip.RelayEvent(pMsg);


    return CMainDialogBase::PreTranslateMessage(pMsg);
}


void CMusicPlayerDlg::OnDestroy()
{
    CMainDialogBase::OnDestroy();

    // TODO: 在此处添加消息处理程序代码

    //获取桌面歌词窗口的位置
    CRect rect;
    ::GetWindowRect(m_desktop_lyric.GetLyricWnd(), rect);
    m_desktop_lyric_pos = rect.TopLeft();
    m_desktop_lyric_size = rect.Size();

    //退出时保存设置
    CPlayer::GetInstance().OnExit();
    SaveConfig();
    m_findDlg.SaveConfig();
    theApp.SaveConfig();
    //解除全局热键
    theApp.m_hot_key.UnRegisterAllHotKey();

    //退出时恢复Cortana的默认文本
    m_cortana_lyric.ResetCortanaText();

    ////退出时删除专辑封面临时文件
    //DeleteFile(CPlayer::GetInstance().GetAlbumCoverPath().c_str());

    m_notify_icon.DeleteNotifyIcon();
}


void CMusicPlayerDlg::OnAppAbout()
{
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}


void CMusicPlayerDlg::OnFileOpen()
{
    // TODO: 在此添加命令处理程序代码
    vector<wstring> files;	//储存打开的多个文件路径
    //设置过滤器
    wstring filter = CAudioCommon::GetFileDlgFilter();

    CCommon::DoOpenFileDlg(filter, files, this);
    if (!files.empty())
    {
        CPlayer::GetInstance().OpenFiles(files);
        UpdatePlayPauseButton();
        DrawInfo(true);
        m_play_error_cnt = 0;
    }
}


void CMusicPlayerDlg::OnFileOpenFolder()
{
    // TODO: 在此添加命令处理程序代码

#ifdef COMPILE_IN_WIN_XP
    CFolderBrowserDlg folderPickerDlg(this->GetSafeHwnd());
    folderPickerDlg.SetInfo(CCommon::LoadText(IDS_OPEN_FOLDER_INFO));
#else
    CFilePathHelper current_path(CPlayer::GetInstance().GetCurrentDir());
    CFolderPickerDialog folderPickerDlg(current_path.GetParentDir().c_str());
#endif
    if (folderPickerDlg.DoModal() == IDOK)
    {
        CPlayer::GetInstance().OpenFolder(wstring(folderPickerDlg.GetPathName()));
        //ShowPlayList();
        UpdatePlayPauseButton();
        //SetPorgressBarSize();
        DrawInfo(true);
        m_play_error_cnt = 0;
    }
}


void CMusicPlayerDlg::OnDropFiles(HDROP hDropInfo)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    vector<wstring> files;	//储存拖放到窗口的多个文件路径
    TCHAR file_path[MAX_PATH];
    int drop_count = DragQueryFile(hDropInfo, -1, NULL, 0);		//取得被拖动文件的数目
    //获取第1个文件
    DragQueryFile(hDropInfo, 0, file_path, MAX_PATH);
    wstring file_path_wcs{ file_path };
    //if (file_path_wcs.size() > 4 && file_path_wcs[file_path_wcs.size() - 4] != L'.' && file_path_wcs[file_path_wcs.size() - 5] != L'.')
    if(CCommon::IsFolder(file_path_wcs))
    {
        //file_path_wcs.push_back(L'\\');
        CPlayer::GetInstance().OpenFolder(file_path_wcs);
    }
    else
    {
        for (int i{}; i < drop_count; i++)
        {
            DragQueryFile(hDropInfo, i, file_path, MAX_PATH);	//获取第i个文件
            if (CAudioCommon::FileIsAudio(wstring(file_path)))
                files.push_back(file_path);
        }
        if(CPlayer::GetInstance().IsPlaylistMode())
            CPlayer::GetInstance().AddFiles(files);
        else
            CPlayer::GetInstance().OpenFiles(files, false);
    }
    //ShowPlayList();
    UpdatePlayPauseButton();
    //SetPorgressBarSize();
    DrawInfo(true);

    CMainDialogBase::OnDropFiles(hDropInfo);
}


void CMusicPlayerDlg::OnInitMenu(CMenu* pMenu)
{
    CMainDialogBase::OnInitMenu(pMenu);

    // TODO: 在此处添加消息处理程序代码
    SetMenuState(pMenu);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    SetMenuState(pSysMenu);
}


void CMusicPlayerDlg::OnPlayOrder()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().SetRepeatMode(RM_PLAY_ORDER);		//设置顺序播放
    m_ui.UpdateRepeatModeToolTip();
    m_ui2.UpdateRepeatModeToolTip();
}


void CMusicPlayerDlg::OnPlayShuffle()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().SetRepeatMode(RM_PLAY_SHUFFLE);		//设置随机播放
    m_ui.UpdateRepeatModeToolTip();
    m_ui2.UpdateRepeatModeToolTip();
}


void CMusicPlayerDlg::OnLoopPlaylist()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().SetRepeatMode(RM_LOOP_PLAYLIST);		//设置列表循环
    m_ui.UpdateRepeatModeToolTip();
    m_ui2.UpdateRepeatModeToolTip();
}


void CMusicPlayerDlg::OnLoopTrack()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().SetRepeatMode(RM_LOOP_TRACK);		//设置单曲循环
    m_ui.UpdateRepeatModeToolTip();
    m_ui2.UpdateRepeatModeToolTip();
}


BOOL CMusicPlayerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (zDelta > 0)
    {
        OnVolumeUp();
    }
    if (zDelta < 0)
    {
        OnVolumeDown();
    }

    return CMainDialogBase::OnMouseWheel(nFlags, zDelta, pt);
}


void CMusicPlayerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    //限制窗口最小大小
    lpMMI->ptMinTrackSize.x = theApp.DPI(340);		//设置最小宽度
    lpMMI->ptMinTrackSize.y = theApp.DPI(360);		//设置最小高度

    CMainDialogBase::OnGetMinMaxInfo(lpMMI);
}


void CMusicPlayerDlg::OnNMDblclkPlaylistList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    if (!m_searched)	//如果播放列表不在搜索状态，则当前选中项的行号就是曲目的索引
    {
        if (pNMItemActivate->iItem < 0)
            return;
        CPlayer::GetInstance().PlayTrack(pNMItemActivate->iItem);
    }
    else		//如果播放列表处理选中状态，则曲目的索引是选中行第一列的数字-1
    {
        int song_index;
        CString str;
        str = m_playlist_list.GetItemText(pNMItemActivate->iItem, 0);
        song_index = _ttoi(str) - 1;
        if (song_index < 0)
            return;
        CPlayer::GetInstance().PlayTrack(song_index);
    }
    SwitchTrack();
    UpdatePlayPauseButton();

    *pResult = 0;
}


//void CMusicPlayerDlg::OnRefreshPlaylist()
//{
//	// TODO: 在此添加命令处理程序代码
//	ShowPlayList();
//}


void CMusicPlayerDlg::OnOptionSettings()
{
    // TODO: 在此添加命令处理程序代码

    theApp.m_hot_key.UnRegisterAllHotKey();

    COptionsDlg optionDlg;
    //初始化对话框中变量的值
    optionDlg.m_tab_selected = m_tab_selected;
    optionDlg.m_tab1_dlg.m_data = theApp.m_lyric_setting_data;
    optionDlg.m_tab1_dlg.m_pDesktopLyric = &m_desktop_lyric;
    if (m_miniModeDlg.m_hWnd == NULL)
        optionDlg.m_tab2_dlg.m_hMainWnd = m_hWnd;
    else
        optionDlg.m_tab2_dlg.m_hMainWnd = m_miniModeDlg.m_hWnd;
    optionDlg.m_tab2_dlg.m_data = theApp.m_app_setting_data;
    optionDlg.m_tab3_dlg.m_data = theApp.m_general_setting_data;
    optionDlg.m_tab4_dlg.m_data = theApp.m_play_setting_data;
    optionDlg.m_tab5_dlg.m_hotkey_group = theApp.m_hot_key.GetHotKeyGroup();
    optionDlg.m_tab5_dlg.m_data = theApp.m_hot_key_setting_data;

    int sprctrum_height = theApp.m_app_setting_data.sprctrum_height;		//保存theApp.m_app_setting_data.sprctrum_height的值，如果用户点击了选项对话框的取消，则需要把恢复为原来的
    int background_transparency = theApp.m_app_setting_data.background_transparency;		//同上
    int desktop_lyric_opacity = theApp.m_lyric_setting_data.desktop_lyric_data.opacity;

    if (optionDlg.DoModal() == IDOK)
    {
        ApplySettings(optionDlg);
    }
    else
    {
        SetTransparency();		//如果点击了取消，则需要重新设置窗口透明度
        SetDesptopLyricTransparency();

        if (m_miniModeDlg.m_hWnd != NULL)
            m_miniModeDlg.SetTransparency();

        theApp.m_app_setting_data.sprctrum_height = sprctrum_height;
        theApp.m_app_setting_data.background_transparency = background_transparency;
        theApp.m_lyric_setting_data.desktop_lyric_data.opacity = desktop_lyric_opacity;
    }

    m_tab_selected = optionDlg.m_tab_selected;

    if(theApp.m_hot_key_setting_data.hot_key_enable)
        theApp.m_hot_key.RegisterAllHotKey();

}


void CMusicPlayerDlg::OnReloadPlaylist()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().ReloadPlaylist();
    //ShowPlayList();
    //UpdatePlayPauseButton();
    //ShowTime();
}


void CMusicPlayerDlg::OnNMRClickPlaylistList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    if (!m_searched)
    {
        m_item_selected = pNMItemActivate->iItem;	//获取鼠标选中的项目
        m_playlist_list.GetItemSelected(m_items_selected);		//获取多个选中的项目
    }
    else
    {
        CString str;
        str = m_playlist_list.GetItemText(pNMItemActivate->iItem, 0);
        m_item_selected = _ttoi(str) - 1;
        m_playlist_list.GetItemSelectedSearched(m_items_selected);
    }

    CMenu* pContextMenu = theApp.m_menu_set.m_list_popup_menu.GetSubMenu(0); //获取第一个弹出菜单
    CPoint point;			//定义一个用于确定光标位置的位置
    GetCursorPos(&point);	//获取当前光标的位置，以便使得菜单可以跟随光标

    if (m_item_selected >= 0 && m_item_selected < CPlayer::GetInstance().GetSongNum())
    {
        CRect item_rect;
        m_playlist_list.GetItemRect(pNMItemActivate->iItem, item_rect, LVIR_BOUNDS);		//获取选中项目的矩形区域（以播放列表控件左上角为原点）
        CRect playlist_rect;
        m_playlist_list.GetWindowRect(playlist_rect);		//获取播放列表控件的矩形区域（以屏幕左上角为原点）
        point.y = playlist_rect.top + item_rect.bottom;	//设置鼠标要弹出的y坐标为选中项目的下边框位置，防止右键菜单挡住选中的项目
    }

    pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this); //在指定位置显示弹出菜单

    *pResult = 0;
}


void CMusicPlayerDlg::OnPlayItem()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().PlayTrack(m_item_selected);
    SwitchTrack();
    UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnItemProperty()
{
    // TODO: 在此添加命令处理程序代码
    CPropertyDlg propertyDlg(CPlayer::GetInstance().GetPlayList());
    propertyDlg.m_index = m_item_selected;
    propertyDlg.m_song_num = CPlayer::GetInstance().GetSongNum();
    //propertyDlg.m_playing_index = CPlayer::GetInstance().GetIndex();
    //propertyDlg.m_lyric_name = CPlayer::GetInstance().GetLyricName();
    propertyDlg.DoModal();
    if (propertyDlg.GetListRefresh())
        ShowPlayList();
}


//void CMusicPlayerDlg::OnRemoveFromPlaylist()
//{
//	// TODO: 在此添加命令处理程序代码
//	CPlayer::GetInstance().RemoveSong(m_item_selected);
//	ShowPlayList();
//	UpdatePlayPauseButton();
//}


//void CMusicPlayerDlg::OnClearPlaylist()
//{
//	// TODO: 在此添加命令处理程序代码
//	CPlayer::GetInstance().ClearPlaylist();
//	ShowPlayList();
//	UpdatePlayPauseButton();
//}


void CMusicPlayerDlg::OnExploreTrack()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().ExplorePath(m_item_selected);
}


BOOL CMusicPlayerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: 在此添加专用代码和/或调用基类
    //响应任务栏缩略图按钮
    WORD command = LOWORD(wParam);
    switch(command)
    {
    case IDT_PLAY_PAUSE:
        OnPlayPause();
        break;
    case IDT_PREVIOUS:
        OnPrevious();
        break;
    case IDT_NEXT:
        OnNext();
        break;
    }

    //响应播放列表右键菜单中的“添加到播放列表”
    if (command >= ID_ADD_TO_DEFAULT_PLAYLIST && command <= ID_ADD_TO_MY_FAVOURITE + ADD_TO_PLAYLIST_MAX_SIZE)
    {
        //获取选中的曲目的路径
        std::vector<std::wstring> selected_item_path;
        for (auto i : m_items_selected)
        {
            if (i >= 0 && i < CPlayer::GetInstance().GetSongNum())
            {
                selected_item_path.push_back(CPlayer::GetInstance().GetPlayList()[i].file_path);
            }
        }

        if (command == ID_ADD_TO_DEFAULT_PLAYLIST)      //添加到默认播放列表
        {
            std::wstring default_playlist_path = CPlayer::GetInstance().GetRecentPlaylist().m_default_playlist.path;
            CPlaylist playlist;
            playlist.LoadFromFile(default_playlist_path);
            playlist.AddFiles(selected_item_path);
            playlist.SaveToFile(default_playlist_path);

        }
        else if (command == ID_ADD_TO_MY_FAVOURITE)      //添加到“我喜欢”播放列表
        {
            std::wstring favourite_playlist_path = CPlayer::GetInstance().GetRecentPlaylist().m_favourite_playlist.path;
            CPlaylist playlist;
            playlist.LoadFromFile(favourite_playlist_path);
            playlist.AddFiles(selected_item_path);
            playlist.SaveToFile(favourite_playlist_path);

            //添加到“我喜欢”播放列表后，为添加的项目设置favourite标记
            for (auto i : m_items_selected)
            {
                if (i >= 0 && i < CPlayer::GetInstance().GetSongNum())
                {
                    CPlayer::GetInstance().GetPlayList()[i].is_favourite = true;
                }
            }

        }
        else        //添加到选中的播放列表
        {
            CString menu_string;
            theApp.m_menu_set.m_list_popup_menu.GetMenuString(command, menu_string, 0);
            if (!menu_string.IsEmpty())
            {
                wstring playlist_path = theApp.m_playlist_dir + menu_string.GetString() + PLAYLIST_EXTENSION;
                if(CCommon::FileExist(playlist_path))
                {
                    CPlaylist playlist;
                    playlist.LoadFromFile(playlist_path);
                    playlist.AddFiles(selected_item_path);
                    playlist.SaveToFile(playlist_path);
                }
                else
                {
                    MessageBox(CCommon::LoadText(IDS_ADD_TO_PLAYLIST_FAILED_WARNING), NULL, MB_ICONWARNING | MB_OK);
                }
            }
        }
    }

    return CMainDialogBase::OnCommand(wParam, lParam);
}


void CMusicPlayerDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    //响应全局快捷键
    switch (nHotKeyId)
    {
    case HK_PLAY_PAUSE:
        OnPlayPause();
        break;
    case HK_PREVIOUS:
        OnPrevious();
        break;
    case HK_NEXT:
        OnNext();
        break;
    case HK_VOLUME_UP:
        OnVolumeUp();
        break;
    case HK_VOLUME_DOWN:
        OnVolumeDown();
        break;
    default:
        break;
    case HK_STOP:
        OnStop();
        break;
    case HK_FF:
        OnFF();
        break;
    case HK_REW:
        OnRew();
        break;
    case HK_EXIT:
        OnMenuExit();
        break;
    }

    CMainDialogBase::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CMusicPlayerDlg::OnReIniBass()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().ReIniPlayerCore();
    UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnSortByFile()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().m_sort_mode = SM_FILE;
    CPlayer::GetInstance().SortPlaylist();
    ShowPlayList();
}


void CMusicPlayerDlg::OnSortByTitle()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().m_sort_mode = SM_TITLE;
    CPlayer::GetInstance().SortPlaylist();
    ShowPlayList();
}


void CMusicPlayerDlg::OnSortByArtist()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().m_sort_mode = SM_ARTIST;
    CPlayer::GetInstance().SortPlaylist();
    ShowPlayList();
}


void CMusicPlayerDlg::OnSortByAlbum()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().m_sort_mode = SM_ALBUM;
    CPlayer::GetInstance().SortPlaylist();
    ShowPlayList();
}


void CMusicPlayerDlg::OnSortByTrack()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().m_sort_mode = SM_TRACK;
    CPlayer::GetInstance().SortPlaylist();
    ShowPlayList();
}


void CMusicPlayerDlg::OnDeleteFromDisk()
{
    // TODO: 在此添加命令处理程序代码
    if (m_item_selected < 0 || m_item_selected >= CPlayer::GetInstance().GetSongNum())
        return;
    int rtn;
    wstring delected_file;
    vector<wstring> delected_files;
    if (m_items_selected.size() > 1)
    {
        CString info;
        info = CCommon::LoadTextFormat(IDS_DELETE_FILE_INQUARY, { m_items_selected.size() });
        if (MessageBox(info, NULL, MB_ICONWARNING | MB_OKCANCEL) != IDOK)
            return;
        if(CCommon::IsItemInVector(m_items_selected, CPlayer::GetInstance().GetIndex()))	//如果选中的文件中有正在播放的文件，则删除前必须先关闭文件
            CPlayer::GetInstance().MusicControl(Command::CLOSE);
        for (const auto& index : m_items_selected)
            delected_files.push_back(CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetPlayList()[index].file_name);
        rtn = CCommon::DeleteFiles(m_hWnd, delected_files);
    }
    else
    {
        if (m_item_selected == CPlayer::GetInstance().GetIndex())	//如果删除的文件是正在播放的文件，则删除前必须先关闭文件
            CPlayer::GetInstance().MusicControl(Command::CLOSE);
        delected_file = CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetPlayList()[m_item_selected].file_name;
        rtn = CCommon::DeleteAFile(m_hWnd, delected_file);
    }
    if (rtn == 0)
    {
        //如果文件删除成功，同时从播放列表中移除
        if (m_items_selected.size() > 1)
            CPlayer::GetInstance().RemoveSongs(m_items_selected);
        else
            CPlayer::GetInstance().RemoveSong(m_item_selected);
        ShowPlayList(false);
        UpdatePlayPauseButton();
        DrawInfo(true);
        //文件删除后同时删除和文件同名的图片文件和歌词文件
        if (m_items_selected.size() > 1)
        {
            for (auto& file : delected_files)
            {
                CFilePathHelper file_path(file);
                file = file_path.ReplaceFileExtension(L"jpg").c_str();
            }
            CCommon::DeleteFiles(m_hWnd, delected_files);
            for (auto& file : delected_files)
            {
                CFilePathHelper file_path(file);
                file = file_path.ReplaceFileExtension(L"lrc").c_str();
            }
            CCommon::DeleteFiles(m_hWnd, delected_files);
        }
        else
        {
            CFilePathHelper file_path(delected_file);
            CCommon::DeleteAFile(m_hWnd, file_path.ReplaceFileExtension(L"jpg").c_str());
            CCommon::DeleteAFile(m_hWnd, file_path.ReplaceFileExtension(L"lrc").c_str());
        }
    }
    else if (rtn == 1223)	//如果在弹出的对话框中点击“取消”则返回值为1223
    {
        if (m_item_selected == CPlayer::GetInstance().GetIndex())		//如果删除的文件是正在播放的文件，又点击了“取消”，则重新打开当前文件
        {
            CPlayer::GetInstance().MusicControl(Command::OPEN);
            CPlayer::GetInstance().MusicControl(Command::SEEK);
            //CPlayer::GetInstance().Refresh();
            UpdatePlayPauseButton();
            DrawInfo(true);
        }
    }
    else
    {
        MessageBox(CCommon::LoadText(IDS_CONNOT_DELETE_FILE), NULL, MB_ICONWARNING);
    }
}


afx_msg LRESULT CMusicPlayerDlg::OnTaskbarcreated(WPARAM wParam, LPARAM lParam)
{
#ifndef COMPILE_IN_WIN_XP
    if (IsTaskbarListEnable())
    {
        //当资源管理器重启后重新添加任务栏缩略图窗口按钮
        m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);
        SetThumbnailClipArea();
        //重新更新任务栏上的播放状态角标
        UpdatePlayPauseButton();
    }
#endif
    //资源管理器重启后Cortana的句柄会发生改变，此时要重新获取Cortana的句柄
    m_cortana_lyric.Init();

    m_notify_icon.AddNotifyIcon();		//重新添加通知区图标

    return 0;
}


void CMusicPlayerDlg::OnDispFileName()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_ui_data.display_format = DF_FILE_NAME;
    ShowPlayList();
}


void CMusicPlayerDlg::OnDispTitle()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_ui_data.display_format = DF_TITLE;
    ShowPlayList();
}


void CMusicPlayerDlg::OnDispArtistTitle()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_ui_data.display_format = DF_ARTIST_TITLE;
    ShowPlayList();
}


void CMusicPlayerDlg::OnDispTitleArtist()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_ui_data.display_format = DF_TITLE_ARTIST;
    ShowPlayList();
}


void CMusicPlayerDlg::OnMiniMode()
{
    // TODO: 在此添加命令处理程序代码
    if (m_miniModeDlg.m_hWnd != NULL)
        return;

    //m_miniModeDlg.SetDefaultBackGround(&theApp.m_ui_data.default_background);
    //m_miniModeDlg.SetDisplayFormat(&theApp.m_ui_data.display_format);
    ShowWindow(SW_HIDE);
    if (m_miniModeDlg.DoModal() == IDCANCEL)
    {
        //SendMessage(WM_COMMAND, ID_APP_EXIT);
        OnMenuExit();
    }
    else
    {
        ShowWindow(SW_SHOW);
#ifndef COMPILE_IN_WIN_XP
        if (IsTaskbarListEnable())
        {
            m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);	//重新添加任务栏缩略图按钮
            SetThumbnailClipArea();		//重新设置任务栏缩略图
        }
#endif
        SetForegroundWindow();
        SwitchTrack();
        UpdatePlayPauseButton();
        SetTransparency();
    }
}


void CMusicPlayerDlg::OnBnClickedStop()
{
    // TODO: 在此添加控件通知处理程序代码
    SetFocus();
    OnStop();
}


void CMusicPlayerDlg::OnBnClickedPrevious()
{
    // TODO: 在此添加控件通知处理程序代码
    SetFocus();
    OnPrevious();
}


void CMusicPlayerDlg::OnBnClickedPlayPause()
{
    // TODO: 在此添加控件通知处理程序代码
    SetFocus();
    OnPlayPause();
}


void CMusicPlayerDlg::OnBnClickedNext()
{
    // TODO: 在此添加控件通知处理程序代码
    SetFocus();
    OnNext();
}


//void CMusicPlayerDlg::OnMove(int x, int y)
//{
//	CMainDialogBase::OnMove(x, y);
//
//	// TODO: 在此处添加消息处理程序代码
//	SetMaskWindowPos();
//}


void CMusicPlayerDlg::OnReloadLyric()
{
    // TODO: 在此添加命令处理程序代码
    CWaitCursor wait_cursor;
    CPlayer::GetInstance().SearchLyrics();
    CPlayer::GetInstance().IniLyrics();
}


void CMusicPlayerDlg::OnSongInfo()
{
    // TODO: 在此添加命令处理程序代码
    CPropertyDlg propertyDlg(CPlayer::GetInstance().GetPlayList());
    propertyDlg.m_index = CPlayer::GetInstance().GetIndex();
    propertyDlg.m_song_num = CPlayer::GetInstance().GetSongNum();
    propertyDlg.DoModal();
    if (propertyDlg.GetListRefresh())
        ShowPlayList();
}


void CMusicPlayerDlg::OnCopyCurrentLyric()
{
    // TODO: 在此添加命令处理程序代码
    bool midi_lyric{ CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !CPlayer::GetInstance().MidiNoLyric() };
    wstring lyric_str;
    if (midi_lyric)
    {
        lyric_str = CPlayer::GetInstance().GetMidiLyric();
    }
    else
    {
        CLyrics::Lyric lyric{ CPlayer::GetInstance().m_Lyrics.GetLyric(Time(CPlayer::GetInstance().GetCurrentPosition()), 0) };
        lyric_str = lyric.text;
        if (theApp.m_ui_data.show_translate && !lyric.translate.empty())
        {
            lyric_str += L"\r\n";
            lyric_str += lyric.translate;
        }
    }
    if (!CCommon::CopyStringToClipboard(lyric_str))
        //	MessageBox(_T("当前歌词已成功复制到剪贴板。"), NULL, MB_ICONINFORMATION);
        //else
        MessageBox(CCommon::LoadText(IDS_COPY_CLIPBOARD_FAILED), NULL, MB_ICONWARNING);
}


void CMusicPlayerDlg::OnCopyAllLyric()
{
    // TODO: 在此添加命令处理程序代码
    if (CCommon::CopyStringToClipboard(CPlayer::GetInstance().m_Lyrics.GetAllLyricText(theApp.m_ui_data.show_translate)))
        MessageBox(CCommon::LoadText(IDS_ALL_LRYIC_COPIED), NULL, MB_ICONINFORMATION);
    else
        MessageBox(CCommon::LoadText(IDS_COPY_CLIPBOARD_FAILED), NULL, MB_ICONWARNING);
}


void CMusicPlayerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    ////双击鼠标左键进入迷你模式
    //if (!theApp.m_ui_data.repetemode_rect.PtInRect(point) && !theApp.m_ui_data.volume_btn.rect.PtInRect(point)
    //	&& !theApp.m_ui_data.volume_up_rect.PtInRect(point) && !theApp.m_ui_data.volume_down_rect.PtInRect(point)
    //	&& !theApp.m_ui_data.translate_btn.rect.PtInRect(point))
    //	OnMiniMode();
    CMainDialogBase::OnLButtonDblClk(nFlags, point);
}


void CMusicPlayerDlg::OnLyricForward()
{
    // TODO: 在此添加命令处理程序代码
    //歌词提前0.5秒
    CPlayer::GetInstance().m_Lyrics.AdjustLyric(-500);

}


void CMusicPlayerDlg::OnLyricDelay()
{
    // TODO: 在此添加命令处理程序代码
    //歌词延后0.5秒
    CPlayer::GetInstance().m_Lyrics.AdjustLyric(500);
}


void CMusicPlayerDlg::OnSaveModifiedLyric()
{
    // TODO: 在此添加命令处理程序代码
    if (theApp.m_lyric_setting_data.save_lyric_in_offset && !CPlayer::GetInstance().m_Lyrics.IsChineseConverted())		//如果执行了中文繁简转换，则保存时不管选项设置如何都调用SaveLyric2()
        CPlayer::GetInstance().m_Lyrics.SaveLyric();
    else
        CPlayer::GetInstance().m_Lyrics.SaveLyric2();
}


void CMusicPlayerDlg::OnEditLyric()
{
    // TODO: 在此添加命令处理程序代码
    //ShellExecute(NULL, _T("open"), CPlayer::GetInstance().m_Lyrics.GetPathName().c_str(), NULL, NULL, SW_SHOWNORMAL);
    CCommon::DeleteModelessDialog(m_pLyricEdit);
    m_pLyricEdit = new CLyricEditDlg;
    m_pLyricEdit->Create(IDD_LYRIC_EDIT_DIALOG);
    m_pLyricEdit->ShowWindow(SW_SHOW);
}


void CMusicPlayerDlg::OnDownloadLyric()
{
    // TODO: 在此添加命令处理程序代码
    CLyricDownloadDlg aDlg;
    aDlg.DoModal();
}


void CMusicPlayerDlg::OnLyricBatchDownload()
{
    // TODO: 在此添加命令处理程序代码
    //CLyricBatchDownloadDlg aDlg;
    //aDlg.DoModal();
    CCommon::DeleteModelessDialog(m_pLyricBatchDownDlg);
    m_pLyricBatchDownDlg = new CLyricBatchDownloadDlg;
    m_pLyricBatchDownDlg->Create(IDD_LYRIC_BATCH_DOWN_DIALOG);
    m_pLyricBatchDownDlg->ShowWindow(SW_SHOW);

}


void CMusicPlayerDlg::OnDeleteLyric()
{
    // TODO: 在此添加命令处理程序代码
    if (CCommon::FileExist(CPlayer::GetInstance().m_Lyrics.GetPathName()))
    {
        int rtn = CCommon::DeleteAFile(m_hWnd, CPlayer::GetInstance().m_Lyrics.GetPathName());		//删除歌词文件
        CPlayer::GetInstance().ClearLyric();		//清除歌词关联
    }

    SongInfo& song_info{ theApp.m_song_data[CPlayer::GetInstance().GetCurrentFilePath()] };
    song_info.no_online_lyric = true;
    theApp.SetSongDataModified();
}


void CMusicPlayerDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (nFlags == MK_SHIFT)		//按住Shift键点击鼠标右键时，弹出系统菜单
    {
        CPoint point1;
        GetCursorPos(&point1);
        theApp.m_menu_set.m_main_menu_popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);
    }
    else
    {
        m_pUI->RButtonUp(point);
    }

    CMainDialogBase::OnRButtonUp(nFlags, point);
}


void CMusicPlayerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    m_pUI->MouseMove(point);

    CMainDialogBase::OnMouseMove(nFlags, point);
}


void CMusicPlayerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (!m_no_lbtnup)
        m_pUI->LButtonUp(point);

    CMainDialogBase::OnLButtonUp(nFlags, point);
}


void CMusicPlayerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    m_pUI->LButtonDown(point);

    CMainDialogBase::OnLButtonDown(nFlags, point);
}


HBRUSH CMusicPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CMainDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  在此更改 DC 的任何特性

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    if (pWnd == this /*|| pWnd == &m_path_static*/)
    {
        static HBRUSH hBackBrush{};
        if (hBackBrush == NULL)
            hBackBrush = CreateSolidBrush(CONSTVAL::BACKGROUND_COLOR);
        return hBackBrush;
    }

    return hbr;
}


afx_msg LRESULT CMusicPlayerDlg::OnPlaylistIniComplate(WPARAM wParam, LPARAM lParam)
{
    CPlayer::GetInstance().IniPlaylistComplate();
    theApp.DoWaitCursor(0);
    ShowPlayList();
    //ShowTime();
    DrawInfo(true);
    //SetPorgressBarSize();
    UpdatePlayPauseButton();
    //ShowTime();

    m_ui.UpdateSongInfoToolTip();
    m_ui2.UpdateSongInfoToolTip();

    EnablePlaylist(true);
    theApp.DoWaitCursor(-1);

    SetPlaylistDragEnable();

    return 0;
}


afx_msg LRESULT CMusicPlayerDlg::OnSetTitle(WPARAM wParam, LPARAM lParam)
{
    CString title;
    title = CPlayListCtrl::GetDisplayStr(CPlayer::GetInstance().GetCurrentSongInfo(), theApp.m_ui_data.display_format).c_str();

    CString title_suffix;
    if (!title.IsEmpty())
        title_suffix += _T(" - ");
    title_suffix += APP_NAME;

    if (CPlayer::GetInstance().IsMciCore())
        title_suffix += _T(" (MCI)");

#ifdef _DEBUG
    title_suffix += _T(' ');
    title_suffix += CCommon::LoadText(IDS_DEBUG_MODE);
#endif

    SetWindowText(title + title_suffix);		//用当前正在播放的歌曲名作为窗口标题

    int title_length = 128 - title_suffix.GetLength() - 1;
    if(title.GetLength() > title_length)
        title = title.Left(title_length);
    m_notify_icon.SetIconToolTip(title + title_suffix);

    return 0;
}


void CMusicPlayerDlg::OnEqualizer()
{
    // TODO: 在此添加命令处理程序代码
    if (CPlayer::GetInstance().IsMciCore())
    {
        MessageBox(CCommon::LoadText(IDS_MCI_NO_THIS_FUNCTION_WARNING), NULL, MB_ICONWARNING | MB_OK);
        return;
    }

    CCommon::DeleteModelessDialog(m_pSoundEffecDlg);
    m_pSoundEffecDlg = new CSoundEffectDlg;
    m_pSoundEffecDlg->Create(IDD_SOUND_EFFECT_DIALOG);
    m_pSoundEffecDlg->ShowWindow(SW_SHOW);
}


void CMusicPlayerDlg::OnExploreOnline()
{
    // TODO: 在此添加命令处理程序代码
    m_pThread = AfxBeginThread(ViewOnlineThreadFunc, (void*)m_item_selected);
}

UINT CMusicPlayerDlg::ViewOnlineThreadFunc(LPVOID lpParam)
{
    CCommon::SetThreadLanguage(theApp.m_general_setting_data.language);
    //此命令用于跳转到歌曲对应的网易云音乐的在线页面
    int item_selected = (int)lpParam;
    if (item_selected >= 0 && item_selected < CPlayer::GetInstance().GetSongNum())
    {
        //查找歌曲并获取最佳匹配项的歌曲ID
        const SongInfo& song{ CPlayer::GetInstance().GetPlayList()[item_selected] };
        if (song.song_id.empty())		//如果没有获取过ID，则获取一次ID
        {
            wstring song_id;
            song_id = CInternetCommon::SearchSongAndGetMatched(song.title, song.artist, song.album, song.file_name).id;
            CPlayer::GetInstance().SetRelatedSongID(item_selected, song_id);
        }

        if (song.song_id.empty())
            return 0;
        //获取网易云音乐中该歌曲的在线接听网址
        wstring song_url{ L"http://music.163.com/#/song?id=" + song.song_id };

        //打开超链接
        ShellExecute(NULL, _T("open"), song_url.c_str(), NULL, NULL, SW_SHOW);
    }
    return 0;
}

UINT CMusicPlayerDlg::DownloadLyricAndCoverThreadFunc(LPVOID lpParam)
{
    CCommon::SetThreadLanguage(theApp.m_general_setting_data.language);
    //CMusicPlayerDlg* pThis = (CMusicPlayerDlg*)lpParam;
    const SongInfo& song{ CPlayer::GetInstance().GetCurrentSongInfo() };
    if (theApp.m_general_setting_data.auto_download_only_tag_full)		//设置了“仅当歌曲信息完整进才自动下载”时，如果歌曲标题和艺术家有一个为空，则不自动下载
    {
        if (song.IsTitleEmpty() || song.IsArtistEmpty())
            return 0;
    }

    SongInfo& song_info_ori{ theApp.m_song_data[song.file_path] };
    bool download_cover{ theApp.m_general_setting_data.auto_download_album_cover && !CPlayer::GetInstance().AlbumCoverExist() && !CPlayer::GetInstance().GetCurrentSongInfo().is_cue && !song_info_ori.no_online_album_cover };
    bool midi_lyric{ CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric };
    bool download_lyric{ theApp.m_general_setting_data.auto_download_lyric && CPlayer::GetInstance().m_Lyrics.IsEmpty() && !midi_lyric && !song_info_ori.no_online_lyric };
    CInternetCommon::ItemInfo match_item;
    if (download_cover || download_lyric)
    {
		DownloadResult result{};
        if (song.song_id.empty())		//如果没有获取过ID，则获取一次ID
        {
            //搜索歌曲并获取最佳匹配的项目
            match_item = CInternetCommon::SearchSongAndGetMatched(song.title, song.artist, song.album, song.file_name, false, &result);
            CPlayer::GetInstance().SetRelatedSongID(match_item.id);
        }
        if (song.song_id.empty())
        {
            if(result == DR_DOWNLOAD_ERROR)     //如果搜索歌曲失败，则标记为没有在线歌词和专辑封面
            {
                song_info_ori.no_online_album_cover = true;
                song_info_ori.no_online_lyric = true;
                theApp.SetSongDataModified();
            }
            return 0;
        }
    }
    //自动下载专辑封面
    if (download_cover && !CPlayer::GetInstance().IsOsuFile())
    {
        wstring cover_url = CCoverDownloadCommon::GetAlbumCoverURL(song.song_id);
        if (cover_url.empty())
        {
            song_info_ori.no_online_album_cover = true;
            theApp.SetSongDataModified();
            return 0;
        }

        //获取要保存的专辑封面的文件路径
        CFilePathHelper cover_file_path;
        if (match_item.album == song.album)		//如果在线搜索结果的唱片集名称和歌曲的相同，则以“唱片集”为文件名保存
        {
            wstring album_name{ match_item.album };
            CCommon::FileNameNormalize(album_name);
            cover_file_path.SetFilePath(CPlayer::GetInstance().GetCurrentDir() + album_name);
        }
        else				//否则以歌曲文件名为文件名保存
        {
            cover_file_path.SetFilePath(CPlayer::GetInstance().GetCurrentDir() + song.file_name);
        }
        CFilePathHelper url_path(cover_url);
        cover_file_path.ReplaceFileExtension(url_path.GetFileExtension().c_str());

        //下面专辑封面
        URLDownloadToFile(0, cover_url.c_str(), cover_file_path.GetFilePath().c_str(), 0, NULL);

        //将下载的专辑封面改为隐藏属性
        SetFileAttributes(cover_file_path.GetFilePath().c_str(), FILE_ATTRIBUTE_HIDDEN);

        //重新从本地获取专辑封面
        CPlayer::GetInstance().SearchOutAlbumCover();
        ::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_ALBUM_COVER_DOWNLOAD_COMPLETE, 0, 0);
    }
    //自动下载歌词
    if (download_lyric)
    {
        //下载歌词
        wstring lyric_str;
        if (!CLyricDownloadCommon::DownloadLyric(song.song_id, lyric_str, true))
        {
            song_info_ori.no_online_lyric = true;
            theApp.SetSongDataModified();
            return 0;
        }
        if (!CLyricDownloadCommon::DisposeLryic(lyric_str))
        {
            song_info_ori.no_online_lyric = true;
            theApp.SetSongDataModified();
            return 0;
        }
        CLyricDownloadCommon::AddLyricTag(lyric_str, match_item.id, match_item.title, match_item.artist, match_item.album);
        //保存歌词
        CFilePathHelper lyric_path;
        wstring file_name;
        if (!song.is_cue)
            file_name = song.file_name;
        else
            file_name = song.artist + L" - " + song.title;
        //if (CCommon::FolderExist(theApp.m_lyric_setting_data.lyric_path))
        //{
        //	lyric_path.SetFilePath(theApp.m_lyric_setting_data.lyric_path + CPlayer::GetInstance().GetCurrentSongInfo().file_name);
        //}
        //else
        //{
        lyric_path.SetFilePath(CPlayer::GetInstance().GetCurrentDir() + file_name);
        //}
        lyric_path.ReplaceFileExtension(L"lrc");
        string _lyric_str = CCommon::UnicodeToStr(lyric_str, CodeType::UTF8);
        ofstream out_put{ lyric_path.GetFilePath(), std::ios::binary };
        out_put << _lyric_str;
        out_put.close();
        //处理歌词翻译
        CLyrics lyrics{ lyric_path.GetFilePath() };		//打开保存过的歌词
        lyrics.DeleteRedundantLyric();		//删除多余的歌词
        lyrics.CombineSameTimeLyric();		//将歌词翻译和原始歌词合并成一句
        lyrics.SaveLyric2();

        CPlayer::GetInstance().IniLyrics(lyric_path.GetFilePath());
    }
    return 0;
}



afx_msg LRESULT CMusicPlayerDlg::OnPlaylistIniStart(WPARAM wParam, LPARAM lParam)
{
    EnablePlaylist(false);
    theApp.DoWaitCursor(1);
    return 0;
}


void CMusicPlayerDlg::OnBrowseLyric()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().ExploreLyric();
}


void CMusicPlayerDlg::OnTranslateToSimplifiedChinese()
{
    // TODO: 在此添加命令处理程序代码
    CWaitCursor wait_cursor;
    CPlayer::GetInstance().m_Lyrics.ChineseConvertion(true);
}


void CMusicPlayerDlg::OnTranslateToTranditionalChinese()
{
    // TODO: 在此添加命令处理程序代码
    CWaitCursor wait_cursor;
    CPlayer::GetInstance().m_Lyrics.ChineseConvertion(false);
}


void CMusicPlayerDlg::OnAlbumCoverSaveAs()
{
    // TODO: 在此添加命令处理程序代码
    //设置过滤器
    CString szFilter = CCommon::LoadText(IDS_ALL_FILES, _T("(*.*)|*.*||"));
    //设置另存为时的默认文件名
    CString file_name;
    CString extension;
    if (CPlayer::GetInstance().IsInnerCover())
    {
        switch (CPlayer::GetInstance().GetAlbumCoverType())
        {
        case 0:
            extension = _T("jpg");
            break;
        case 1:
            extension = _T("png");
            break;
        case 2:
            extension = _T("gif");
            break;
        default:
            return;
        }
    }
    else
    {
        CFilePathHelper cover_path(CPlayer::GetInstance().GetAlbumCoverPath());
        extension = cover_path.GetFileExtension().c_str();
    }
    file_name.Format(_T("AlbumCover - %s - %s.%s"), CPlayer::GetInstance().GetCurrentSongInfo().artist.c_str(), CPlayer::GetInstance().GetCurrentSongInfo().album.c_str(), extension);
    wstring file_name_wcs{ file_name };
    CCommon::FileNameNormalize(file_name_wcs);		//替换掉文件名中的无效字符
    //构造保存文件对话框
    CFileDialog fileDlg(FALSE, _T("txt"), file_name_wcs.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
    //显示保存文件对话框
    if (IDOK == fileDlg.DoModal())
    {
        CString dest_file = fileDlg.GetPathName();
        ::CopyFile(CPlayer::GetInstance().GetAlbumCoverPath().c_str(), dest_file, FALSE);
        SetFileAttributes(dest_file, FILE_ATTRIBUTE_NORMAL);		//取消文件的隐藏属性
    }
}


afx_msg LRESULT CMusicPlayerDlg::OnConnotPlayWarning(WPARAM wParam, LPARAM lParam)
{
    if (theApp.m_nc_setting_data.no_sf2_warning)
    {
        if (MessageBox(CCommon::LoadText(IDS_NO_MIDI_SF2_WARNING), NULL, MB_ICONWARNING | MB_OKCANCEL) == IDCANCEL)
            theApp.m_nc_setting_data.no_sf2_warning = false;
    }
    return 0;
}


void CMusicPlayerDlg::OnEnChangeSearchEdit()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CMainDialogBase::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    CString key_word;
    m_search_edit.GetWindowText(key_word);
    m_searched = (key_word.GetLength() != 0);
    SetPlaylistDragEnable();
    m_playlist_list.QuickSearch(wstring(key_word));
    m_playlist_list.ShowPlaylist(theApp.m_ui_data.display_format, m_searched);
}


void CMusicPlayerDlg::OnBnClickedClearSearchButton()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_searched)
    {
        //清除搜索结果
        m_searched = false;
        m_search_edit.SetWindowText(_T(""));
        m_playlist_list.ShowPlaylist(theApp.m_ui_data.display_format, m_searched);
        m_playlist_list.EnsureVisible(CPlayer::GetInstance().GetIndex(), FALSE);		//清除搜索结果后确保正在播放曲目可见
    }
}


void CMusicPlayerDlg::OnDownloadAlbumCover()
{
    // TODO: 在此添加命令处理程序代码
    //_OnDownloadAlbumCover(true);
    CCoverDownloadDlg dlg;
    dlg.DoModal();
}


afx_msg LRESULT CMusicPlayerDlg::OnMusicStreamOpened(WPARAM wParam, LPARAM lParam)
{
    //专辑封面高斯模糊处理（放到这里是为了避免此函数在工作线程中被调用，在工作线程中，拉伸图片的处理CDrawCommon::BitmapStretch有一定的概率出错，原因未知）
    CPlayer::GetInstance().AlbumCoverGaussBlur();
    //自动下载专辑封面
    m_pDownloadThread = AfxBeginThread(DownloadLyricAndCoverThreadFunc, this);

	m_desktop_lyric.ClearLyric();

    return 0;
}


void CMusicPlayerDlg::OnCurrentExploreOnline()
{
    // TODO: 在此添加命令处理程序代码
    m_pThread = AfxBeginThread(ViewOnlineThreadFunc, (void*)CPlayer::GetInstance().GetIndex());
}


void CMusicPlayerDlg::OnDeleteAlbumCover()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().DeleteAlbumCover();
    SongInfo& song_info{ theApp.m_song_data[CPlayer::GetInstance().GetCurrentFilePath()] };
    song_info.no_online_album_cover = true;
    theApp.SetSongDataModified();
}


void CMusicPlayerDlg::OnCopyFileTo()
{
    // TODO: 在此添加命令处理程序代码
    LPCTSTR title{ CCommon::LoadText(IDS_SELECT_COPY_TARGET_FOLDER) };
#ifdef COMPILE_IN_WIN_XP
    CFolderBrowserDlg folderPickerDlg(this->GetSafeHwnd());
    folderPickerDlg.SetInfo(title);
#else
    CFilePathHelper current_path(CPlayer::GetInstance().GetCurrentDir());
    CFolderPickerDialog folderPickerDlg(current_path.GetParentDir().c_str());
    folderPickerDlg.m_ofn.lpstrTitle = title;
#endif
    if (folderPickerDlg.DoModal() == IDOK)
    {
        if (m_item_selected < 0 || m_item_selected >= CPlayer::GetInstance().GetSongNum())
            return;
        if (m_items_selected.size() > 1)
        {
            vector<wstring> source_files;
            for (const auto& index : m_items_selected)
                source_files.push_back(CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetPlayList()[index].file_name);
            CCommon::CopyFiles(this->GetSafeHwnd(), source_files, wstring(folderPickerDlg.GetPathName()));
        }
        else
        {
            wstring source_file = CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetPlayList()[m_item_selected].file_name;
            CCommon::CopyAFile(this->GetSafeHwnd(), CPlayer::GetInstance().GetCurrentFilePath(), wstring(folderPickerDlg.GetPathName()));
        }
    }
}


void CMusicPlayerDlg::OnMoveFileTo()
{
    // TODO: 在此添加命令处理程序代码
    LPCTSTR title{ CCommon::LoadText(IDS_SELECT_MOVE_TARGET_FOLDER) };
#ifdef COMPILE_IN_WIN_XP
    CFolderBrowserDlg folderPickerDlg(this->GetSafeHwnd());
    folderPickerDlg.SetInfo(title);
#else
    CFilePathHelper current_path(CPlayer::GetInstance().GetCurrentDir());
    CFolderPickerDialog folderPickerDlg(current_path.GetParentDir().c_str());
    folderPickerDlg.m_ofn.lpstrTitle = title;
#endif
    if (folderPickerDlg.DoModal() == IDOK)
    {
        if (m_item_selected < 0 || m_item_selected >= CPlayer::GetInstance().GetSongNum())
            return;
        wstring source_file;
        vector<wstring> source_files;
        int rtn;
        if (m_items_selected.size() > 1)
        {
            if (CCommon::IsItemInVector(m_items_selected, CPlayer::GetInstance().GetIndex()))	//如果选中的文件中有正在播放的文件，则移动前必须先关闭文件
                CPlayer::GetInstance().MusicControl(Command::CLOSE);
            for (const auto& index : m_items_selected)
                source_files.push_back(CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetPlayList()[index].file_name);
            rtn = CCommon::MoveFiles(m_hWnd, source_files, wstring(folderPickerDlg.GetPathName()));
        }
        else
        {
            if (m_item_selected == CPlayer::GetInstance().GetIndex())	//如果移动的文件是正在播放的文件，则移动前必须先关闭文件
                CPlayer::GetInstance().MusicControl(Command::CLOSE);
            source_file = CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetPlayList()[m_item_selected].file_name;
            rtn = CCommon::MoveAFile(m_hWnd, source_file, wstring(folderPickerDlg.GetPathName()));
        }
        if (rtn == 0)
        {
            //如果文件移动成功，同时从播放列表中移除
            if (m_items_selected.size() > 1)
                CPlayer::GetInstance().RemoveSongs(m_items_selected);
            else
                CPlayer::GetInstance().RemoveSong(m_item_selected);
            ShowPlayList();
            UpdatePlayPauseButton();
            DrawInfo(true);
        }
    }
}


afx_msg LRESULT CMusicPlayerDlg::OnOpenFileCommandLine(WPARAM wParam, LPARAM lParam)
{
    wstring cmd_line = CCommon::GetStringFromClipboard();
    if (cmd_line.empty())
        return 0;
    vector<wstring> files;
    CCommon::DisposeCmdLineFiles(wstring(cmd_line), files);
    CPlayer::GetInstance().OpenFiles(files);
    return 0;
}


void CMusicPlayerDlg::OnFormatConvert()
{
    // TODO: 在此添加命令处理程序代码
    if (!theApp.m_format_convert_dialog_exit)
        return;
    CCommon::DeleteModelessDialog(m_pFormatConvertDlg);

    if (CPlayer::GetInstance().IsMciCore())
    {
        MessageBox(CCommon::LoadText(IDS_MCI_NO_THIS_FUNCTION_WARNING), NULL, MB_ICONWARNING | MB_OK);
        return;
    }

    m_pFormatConvertDlg = new CFormatConvertDlg(m_items_selected);
    m_pFormatConvertDlg->Create(IDD_FORMAT_CONVERT_DIALOG);
    m_pFormatConvertDlg->ShowWindow(SW_SHOW);
}


void CMusicPlayerDlg::OnFormatConvert1()
{
    // TODO: 在此添加命令处理程序代码
    if (!theApp.m_format_convert_dialog_exit)
        return;
    CCommon::DeleteModelessDialog(m_pFormatConvertDlg);

    if (CPlayer::GetInstance().IsMciCore())
    {
        MessageBox(CCommon::LoadText(IDS_MCI_NO_THIS_FUNCTION_WARNING), NULL, MB_ICONWARNING | MB_OK);
        return;
    }

    vector<int> items_selected;
    items_selected.push_back(CPlayer::GetInstance().GetIndex());
    m_pFormatConvertDlg = new CFormatConvertDlg(items_selected);
    m_pFormatConvertDlg->Create(IDD_FORMAT_CONVERT_DIALOG);
    m_pFormatConvertDlg->ShowWindow(SW_SHOW);
}


afx_msg LRESULT CMusicPlayerDlg::OnSettingsApplied(WPARAM wParam, LPARAM lParam)
{
    COptionsDlg* pOptionsDlg = (COptionsDlg*)wParam;
    if (pOptionsDlg == nullptr)
        return 0;
    ApplySettings(*pOptionsDlg);
    return 0;
}


void CMusicPlayerDlg::OnRecorder()
{
    // TODO: 在此添加命令处理程序代码
    CRecorderDlg dlg;
    dlg.DoModal();
}


afx_msg LRESULT CMusicPlayerDlg::OnAlbumCoverDownloadComplete(WPARAM wParam, LPARAM lParam)
{
    //由于此函数放到线程中处理时，拉伸图片的处理CDrawCommon::BitmapStretch有一定的概率出错，原因未知
    //导致专辑封面背景是黑色的，因此通过发送消息放到主线程中处理
    CPlayer::GetInstance().AlbumCoverGaussBlur();

    if(theApp.m_nc_setting_data.show_cover_tip)
    {
        m_ui.UpdateSongInfoToolTip();
        m_ui2.UpdateSongInfoToolTip();
    }

    return 0;
}


void CMusicPlayerDlg::OnColorizationColorChanged(DWORD dwColorizationColor, BOOL bOpacity)
{
    // 此功能要求 Windows Vista 或更高版本。
    // _WIN32_WINNT 符号必须 >= 0x0600。
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    //响应主题颜色改变消息
    ThemeColorChanged();

    CMainDialogBase::OnColorizationColorChanged(dwColorizationColor, bOpacity);
}


void CMusicPlayerDlg::OnSupportedFormat()
{
    // TODO: 在此添加命令处理程序代码
    CSupportedFormatDlg dlg;
    dlg.DoModal();
}


void CMusicPlayerDlg::OnSwitchUi()
{
    // TODO: 在此添加命令处理程序代码

    if (m_pUI == &m_ui)
    {
        m_pUI = &m_ui2;
        m_ui.ClearBtnRect();
        m_ui.UpdateToolTipPosition();
    }
    else
    {
        m_pUI = &m_ui;
        m_ui2.ClearBtnRect();
        m_ui2.UpdateToolTipPosition();
    }

    DrawInfo(true);
    SetThumbnailClipArea();
    m_ui.UpdateRepeatModeToolTip();
    m_ui2.UpdateRepeatModeToolTip();
    m_pUI->UpdateToolTipPosition();
}

void CMusicPlayerDlg::OnVolumeUp()
{
    if (m_miniModeDlg.m_hWnd == NULL)
        CPlayer::GetInstance().MusicControl(Command::VOLUME_UP, theApp.m_nc_setting_data.volum_step);
    else
        m_miniModeDlg.SetVolume(true);
}

void CMusicPlayerDlg::OnVolumeDown()
{
    if (m_miniModeDlg.m_hWnd == NULL)
        CPlayer::GetInstance().MusicControl(Command::VOLUME_DOWN, theApp.m_nc_setting_data.volum_step);
    else
        m_miniModeDlg.SetVolume(false);
}


afx_msg LRESULT CMusicPlayerDlg::OnNotifyicon(WPARAM wParam, LPARAM lParam)
{
    m_notify_icon.OnNotifyIcon(lParam, m_miniModeDlg.m_hWnd);

    if (lParam == WM_LBUTTONUP && m_miniModeDlg.m_hWnd == NULL)
    {
#ifndef COMPILE_IN_WIN_XP
        if (IsTaskbarListEnable())
        {
            m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);	//重新添加任务栏缩略图按钮
            SetThumbnailClipArea();		//重新设置任务栏缩略图
        }
#endif
        UpdatePlayPauseButton();
    }

    return 0;
}


void CMusicPlayerDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CMainDialogBase::OnClose();
}


void CMusicPlayerDlg::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类
    if(theApp.m_general_setting_data.minimize_to_notify_icon)
        this->ShowWindow(HIDE_WINDOW);
    else
        CMainDialogBase::OnCancel();
}


void CMusicPlayerDlg::OnMenuExit()
{
    // TODO: 在此添加命令处理程序代码
    if (m_miniModeDlg.m_hWnd == NULL)
    {
        CMainDialogBase::OnCancel();
    }
    else
    {
        ::SendMessage(m_miniModeDlg.m_hWnd, WM_COMMAND, ID_MINI_MODE_EXIT, NULL);
    }
}


void CMusicPlayerDlg::OnMinimodeRestore()
{
    // TODO: 在此添加命令处理程序代码
    if (m_miniModeDlg.m_hWnd == NULL)
    {
        OnMiniMode();
    }
    else
    {
        ::SendMessage(m_miniModeDlg.m_hWnd, WM_COMMAND, IDOK, NULL);
    }
}


void CMusicPlayerDlg::OnAppCommand(CWnd* pWnd, UINT nCmd, UINT nDevice, UINT nKey)
{
    // 该功能要求使用 Windows 2000 或更高版本。
    // 符号 _WIN32_WINNT 和 WINVER 必须 >= 0x0500。

    //响应多媒体键
    if (!theApp.IsGlobalMultimediaKeyEnabled())	//如果没有设置响应全局的多媒体按键消息，则在当前窗口内响应多媒体按键消息
    {
        switch (nCmd)
        {
        case APPCOMMAND_MEDIA_PLAY_PAUSE:
            OnPlayPause();
            break;
        case APPCOMMAND_MEDIA_PREVIOUSTRACK:
            OnPrevious();
            break;
        case APPCOMMAND_MEDIA_NEXTTRACK:
            OnNext();
            break;
        case APPCOMMAND_MEDIA_STOP:
            OnStop();
            break;
        default:
            break;
        }
    }

    CMainDialogBase::OnAppCommand(pWnd, nCmd, nDevice, nKey);
}



void CMusicPlayerDlg::OnShowPlaylist()
{
    // TODO: 在此添加命令处理程序代码
    m_pUI->ClearInfo();
    theApp.m_ui_data.show_playlist = !theApp.m_ui_data.show_playlist;

    OnSize(SIZE_RESTORED, theApp.m_ui_data.client_width, theApp.m_ui_data.client_height);
    SetPlaylistVisible();

    DrawInfo(true);
}


BOOL CMusicPlayerDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (m_pUI->SetCursor())
        return TRUE;
    else
        return CMainDialogBase::OnSetCursor(pWnd, nHitTest, message);
}


void CMusicPlayerDlg::OnMouseLeave()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    m_pUI->MouseLeave();

    CMainDialogBase::OnMouseLeave();
}


void CMusicPlayerDlg::OnShowMenuBar()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_ui_data.show_menu_bar = !theApp.m_ui_data.show_menu_bar;
    SetMenubarVisible();
    SetThumbnailClipArea();
    DrawInfo();
    m_pUI->UpdateToolTipPosition();

    //隐藏菜单栏后弹出提示，告诉用户如何再次显示菜单栏
    if (!theApp.m_ui_data.show_menu_bar)
    {
        if (theApp.m_nc_setting_data.show_hide_menu_bar_tip)
        {
            if (MessageBox(CCommon::LoadText(IDS_HIDE_MENU_BAR_TIP), NULL, MB_ICONINFORMATION | MB_OKCANCEL) == IDCANCEL)
                theApp.m_nc_setting_data.show_hide_menu_bar_tip = false;
        }
    }
}


void CMusicPlayerDlg::OnFullScreen()
{
    // TODO: 在此添加命令处理程序代码

    if (m_miniModeDlg.m_hWnd != NULL)	//迷你模式下不允许响应全屏显示
        return;

    theApp.m_ui_data.full_screen = !theApp.m_ui_data.full_screen;
    SetFullScreen(theApp.m_ui_data.full_screen);
    DrawInfo(true);
    m_pUI->UpdateToolTipPosition();
    m_pUI->UpdateFullScreenTip();
    SetThumbnailClipArea();
}


void CMusicPlayerDlg::OnCreatePlayShortcut()
{
    // TODO: 在此添加命令处理程序代码

    if (MessageBox(CCommon::LoadText(IDS_CREATE_PLAY_SHORTCUT_INFO), NULL, MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
    {
        //创建播放/暂停快捷方式
        wstring play_pause = CCommon::LoadText(IDS_PLAY_PAUSE, L".lnk");
        CCommon::FileNameNormalize(play_pause);

        bool success = true;
        success &= CCommon::CreateFileShortcut(theApp.m_module_dir.c_str(), NULL, play_pause.c_str(), NULL, 0, 0, 1, L"-play_pause", 2);

        //创建上一曲快捷方式
        success &= CCommon::CreateFileShortcut(theApp.m_module_dir.c_str(), NULL, CCommon::LoadText(IDS_PREVIOUS, L".lnk"), NULL, 0, 0, 1, L"-previous", 1);

        //创建下一曲快捷方式
        success &= CCommon::CreateFileShortcut(theApp.m_module_dir.c_str(), NULL, CCommon::LoadText(IDS_NEXT, L".lnk"), NULL, 0, 0, 1, L"-next", 3);

        //创建停止快捷方式
        success &= CCommon::CreateFileShortcut(theApp.m_module_dir.c_str(), NULL, CCommon::LoadText(IDS_STOP, L".lnk"), NULL, 0, 0, 1, L"-stop", 6);

        if (success)
            MessageBox(CCommon::LoadTextFormat(IDS_PLAY_SHORTCUT_CREATED, { theApp.m_module_dir }), NULL, MB_ICONINFORMATION);
        else
            MessageBox(CCommon::LoadText(IDS_SHORTCUT_CREAT_FAILED), NULL, MB_ICONWARNING);
    }
}


void CMusicPlayerDlg::OnListenStatistics()
{
    // TODO: 在此添加命令处理程序代码
    CListenTimeStatisticsDlg dlg;
    dlg.DoModal();
}


void CMusicPlayerDlg::OnDarkMode()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_app_setting_data.dark_mode = !theApp.m_app_setting_data.dark_mode;
    if (theApp.m_app_setting_data.dark_mode)
        theApp.m_app_setting_data.background_transparency = 50;
    else
        theApp.m_app_setting_data.background_transparency = 80;
    SaveConfig();
}


afx_msg LRESULT CMusicPlayerDlg::OnMainMenuPopup(WPARAM wParam, LPARAM lParam)
{
    CPoint point = *((CPoint*)wParam);
    ClientToScreen(&point);
    theApp.m_menu_set.m_main_menu_popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

    return 0;
}



void CMusicPlayerDlg::OnAlwaysOnTop()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_nc_setting_data.always_on_top = !theApp.m_nc_setting_data.always_on_top;
    SetAlwaysOnTop();
}


void CMusicPlayerDlg::OnFloatPlaylist()
{
    // TODO: 在此添加命令处理程序代码

    theApp.m_nc_setting_data.float_playlist = !theApp.m_nc_setting_data.float_playlist;
    if(theApp.m_nc_setting_data.float_playlist)
    {
        ShowFloatPlaylist();
    }
    else
    {
        HideFloatPlaylist();
    }
}


void CMusicPlayerDlg::OnDockedPlaylist()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_nc_setting_data.playlist_btn_for_float_playlist = false;
}


void CMusicPlayerDlg::OnFloatedPlaylist()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_nc_setting_data.playlist_btn_for_float_playlist = true;
}

LRESULT CMusicPlayerDlg::OnFloatPlaylistClosed(WPARAM wParam, LPARAM lParam)
{
    CRect rect;
    if(m_pFloatPlaylistDlg != nullptr)
    {
        ::GetWindowRect(m_pFloatPlaylistDlg->GetSafeHwnd(), rect);
        if (!m_pFloatPlaylistDlg->IsIconic() && !m_pFloatPlaylistDlg->IsZoomed())
            m_float_playlist_pos = rect.TopLeft();
    }

    return 0;
}


//void CMusicPlayerDlg::OnFileOpenPalylist()
//{
//    //设置过滤器
//    CString szFilter = CCommon::LoadText(IDS_PLAYLIST_FILTER);
//    //构造打开文件对话框
//    CFileDialog fileDlg(TRUE, _T("playlist"), NULL, 0, szFilter, this);
//    //显示打开文件对话框
//    if (IDOK == fileDlg.DoModal())
//    {
//        CPlaylist playlist;
//        playlist.LoadFromFile(wstring(fileDlg.GetPathName()));
//        CPlayer::GetInstance().OpenFiles(playlist.GetPlaylist(), false);
//    }
//
//}


afx_msg LRESULT CMusicPlayerDlg::OnPlaylistSelected(WPARAM wParam, LPARAM lParam)
{
    CSelectPlaylistDlg* pPathDlg = (CSelectPlaylistDlg*)wParam;
    if (pPathDlg != nullptr)
    {
        if(lParam == TRUE)      //当lParam为1时，播放默认的播放列表
        {
            auto default_playlist = CPlayer::GetInstance().GetRecentPlaylist().m_default_playlist;
            CPlayer::GetInstance().SetPlaylist(default_playlist.path, default_playlist.track, default_playlist.position);
        }
        else
        {
            CPlayer::GetInstance().SetPlaylist(pPathDlg->GetSelPlaylistPath(), pPathDlg->GetTrack(), pPathDlg->GetPosition());
        }
        UpdatePlayPauseButton();
        //SetPorgressBarSize();
        //ShowTime();
        DrawInfo(true);
        m_findDlg.ClearFindResult();		//更换路径后清除查找结果
        CPlayer::GetInstance().SaveRecentPath();
        m_play_error_cnt = 0;
        SetTimer(DELAY_TIMER_ID, 500, NULL);        //在媒体库对话框中选择了一个播放列表播放后，500毫秒内不响应WM_LBUTTONUP消息
        m_no_lbtnup = true;
    }
    return 0;
}


void CMusicPlayerDlg::OnPlaylistAddFile()
{
    // TODO: 在此添加命令处理程序代码

    if (!CPlayer::GetInstance().IsPlaylistMode())
        return;

    vector<wstring> files;
    wstring filter = CAudioCommon::GetFileDlgFilter();
    CCommon::DoOpenFileDlg(filter, files, this);
    if(!files.empty())
    {
        CPlayer::GetInstance().AddFiles(files);
        CPlayer::GetInstance().SaveCurrentPlaylist();
    }
}


void CMusicPlayerDlg::OnRemoveFromPlaylist()
{
    // TODO: 在此添加命令处理程序代码
    CPlayer::GetInstance().RemoveSongs(m_items_selected);
    CPlayer::GetInstance().SaveCurrentPlaylist();
    ShowPlayList(false);
}


void CMusicPlayerDlg::OnEmptyPlaylist()
{
    // TODO: 在此添加命令处理程序代码
    if (MessageBox(CCommon::LoadText(IDS_CLEAR_PLAYLIST_WARNING), NULL, MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
    {
        CPlayer::GetInstance().ClearPlaylist();
        CPlayer::GetInstance().SaveCurrentPlaylist();
        ShowPlayList();
    }
}


void CMusicPlayerDlg::OnMovePlaylistItemUp()
{
    // TODO: 在此添加命令处理程序代码
    if (m_items_selected.empty())
        return;

    bool move_enable = CPlayer::GetInstance().IsPlaylistMode() && !m_searched;
    if (!move_enable)
        return;

    int first = m_items_selected.front();
    int last = m_items_selected.back();
    if(CPlayer::GetInstance().MoveUp(first, last))
    {
        ShowPlayList(false);
        if (m_pFloatPlaylistDlg->GetSafeHwnd() == NULL)
        {
            m_playlist_list.SetCurSel(first - 1, last - 1);
            GetPlaylistItemSelected();
        }
        else
        {
            m_pFloatPlaylistDlg->GetListCtrl().SetCurSel(first - 1, last - 1);
            m_pFloatPlaylistDlg->GetPlaylistItemSelected();
        }
    }
}


void CMusicPlayerDlg::OnMovePlaylistItemDown()
{
    // TODO: 在此添加命令处理程序代码
    if (m_items_selected.empty())
        return;

    bool move_enable = CPlayer::GetInstance().IsPlaylistMode() && !m_searched;
    if (!move_enable)
        return;

    int first = m_items_selected.front();
    int last = m_items_selected.back();
    if(CPlayer::GetInstance().MoveDown(first, last))
    {
        ShowPlayList(false);
        if (m_pFloatPlaylistDlg->GetSafeHwnd() == NULL)
        {
            m_playlist_list.SetCurSel(first + 1, last + 1);
            GetPlaylistItemSelected();
        }
        else
        {
            m_pFloatPlaylistDlg->GetListCtrl().SetCurSel(first + 1, last + 1);
            m_pFloatPlaylistDlg->GetPlaylistItemSelected();
        }
    }
}


void CMusicPlayerDlg::OnNMClickPlaylistList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    GetPlaylistItemSelected();
    *pResult = 0;
}


void CMusicPlayerDlg::OnRemoveSameSongs()
{
    // TODO: 在此添加命令处理程序代码
    int removed = CPlayer::GetInstance().RemoveSameSongs();
    if (removed > 0)
    {
        CPlayer::GetInstance().SaveCurrentPlaylist();
        ShowPlayList();
    }
    MessageBox(CCommon::LoadTextFormat(IDS_REMOVE_SAME_SONGS_INFO, { removed }), NULL, MB_ICONINFORMATION | MB_OK);
}


void CMusicPlayerDlg::OnAddToNewPlaylist()
{
    // TODO: 在此添加命令处理程序代码
    CInputDlg imput_dlg;
    imput_dlg.SetTitle(CCommon::LoadText(IDS_NEW_PLAYLIST));
    imput_dlg.SetInfoText(CCommon::LoadText(IDS_INPUT_PLAYLIST_NAME));
    if (imput_dlg.DoModal() == IDOK)
    {
        CString playlist_name = imput_dlg.GetEditText();
        if (!CCommon::IsFileNameValid(wstring(playlist_name.GetString())))
        {
            MessageBox(CCommon::LoadText(IDS_FILE_NAME_INVALID_WARNING), NULL, MB_ICONWARNING | MB_OK);
            return;
        }
        wstring playlist_path = theApp.m_playlist_dir + playlist_name.GetString() + PLAYLIST_EXTENSION;
        if (CCommon::FileExist(playlist_path))
        {
            MessageBox(CCommon::LoadTextFormat(IDS_PLAYLIST_EXIST_WARNING, { playlist_name }), NULL, MB_ICONWARNING | MB_OK);
            return;
        }
        //添加空的播放列表
        CPlayer::GetInstance().GetRecentPlaylist().AddNewPlaylist(playlist_path);
        
        //获取选中的曲目的路径
        std::vector<std::wstring> selected_item_path;
        for (auto i : m_items_selected)
        {
            if (i >= 0 && i < CPlayer::GetInstance().GetSongNum())
            {
                selected_item_path.push_back(CPlayer::GetInstance().GetPlayList()[i].file_path);
            }
        }

        CPlaylist playlist;
        playlist.LoadFromFile(playlist_path);
        playlist.AddFiles(selected_item_path);
        playlist.SaveToFile(playlist_path);

        IniPlaylistPopupMenu();
    }
}


void CMusicPlayerDlg::OnToolFileRelate()
{
    // TODO: 在此添加命令处理程序代码
    CFileRelateDlg dlg;
    dlg.DoModal();
}


void CMusicPlayerDlg::OnPlaylistAddFolder()
{
    // TODO: 在此添加命令处理程序代码

    if (!CPlayer::GetInstance().IsPlaylistMode())
        return;

    static bool include_sub_dir{ false };
#ifdef COMPILE_IN_WIN_XP
    CFolderBrowserDlg folderPickerDlg(this->GetSafeHwnd());
    folderPickerDlg.SetInfo(CCommon::LoadText(IDS_OPEN_FOLDER_INFO));
#else
    CFilePathHelper current_path(CPlayer::GetInstance().GetCurrentDir());
    CFolderPickerDialog folderPickerDlg(current_path.GetParentDir().c_str());
    folderPickerDlg.AddCheckButton(IDC_OPEN_CHECKBOX, CCommon::LoadText(IDS_INCLUDE_SUB_DIR), include_sub_dir);     //在打开对话框中添加一个复选框
#endif
    if (folderPickerDlg.DoModal() == IDOK)
    {
#ifndef COMPILE_IN_WIN_XP
        BOOL checked;
        folderPickerDlg.GetCheckButtonState(IDC_OPEN_CHECKBOX, checked);
        include_sub_dir = (checked != FALSE);
#endif
        std::vector<wstring> file_list;
        CAudioCommon::GetAudioFiles(wstring(folderPickerDlg.GetPathName()), file_list, MAX_SONG_NUM, include_sub_dir);
        CPlayer::GetInstance().AddFiles(file_list);
        CPlayer::GetInstance().SaveCurrentPlaylist();

    }
}


void CMusicPlayerDlg::OnRemoveInvalidItems()
{
    // TODO: 在此添加命令处理程序代码
    int removed = CPlayer::GetInstance().RemoveInvalidSongs();
    if (removed > 0)
    {
        CPlayer::GetInstance().SaveCurrentPlaylist();
        ShowPlayList();
    }
    MessageBox(CCommon::LoadTextFormat(IDS_REMOVE_SAME_SONGS_INFO, { removed }), NULL, MB_ICONINFORMATION | MB_OK);
}


afx_msg LRESULT CMusicPlayerDlg::OnListItemDragged(WPARAM wParam, LPARAM lParam)
{
    if (!CPlayer::GetInstance().IsPlaylistMode())
        return 0;

    CWaitCursor wait_cursor;
    int drop_index = static_cast<int>(wParam);
    std::vector<int> drag_items;
    if (m_pFloatPlaylistDlg->GetSafeHwnd() == NULL)
        m_playlist_list.GetItemSelected(drag_items);
    else
        m_pFloatPlaylistDlg->GetListCtrl().GetItemSelected(drag_items);

    if (m_miniModeDlg.GetSafeHwnd()!=NULL)
        m_miniModeDlg.GetPlaylistCtrl().GetItemSelected(drag_items);

    int index = CPlayer::GetInstance().MoveItems(drag_items, drop_index);
    ShowPlayList(false);

    //移动后设置当前选中行
    if (m_pFloatPlaylistDlg->GetSafeHwnd() == NULL)
    {
        m_playlist_list.SetCurSel(index, index + drag_items.size() - 1);
        GetPlaylistItemSelected();
    }
    else
    {
        m_pFloatPlaylistDlg->GetListCtrl().SetCurSel(index, index + drag_items.size() - 1);
        m_pFloatPlaylistDlg->GetPlaylistItemSelected();
    }

    if (m_miniModeDlg.GetSafeHwnd() != NULL)
        m_miniModeDlg.GetPlaylistCtrl().SetCurSel(index, index + drag_items.size() - 1);

    return 0;
}


void CMusicPlayerDlg::OnAddRemoveFromFavourite()
{
    // TODO: 在此添加命令处理程序代码
    if (CPlayer::GetInstance().IsPlaylistMode() && CPlayer::GetInstance().GetRecentPlaylist().m_cur_playlist_type == PT_FAVOURITE)
    {
        //如果当前播放列表就是“我喜欢”播放列表，则直接将当前歌曲从列表中移除
        if(MessageBox(CCommon::LoadText(IDS_REMOVE_MY_FAVOURITE_WARNING), NULL, MB_ICONINFORMATION | MB_OKCANCEL) == IDOK)
        {
            CPlayer::GetInstance().RemoveSong(CPlayer::GetInstance().GetIndex());
            CPlayer::GetInstance().SaveCurrentPlaylist();
            ShowPlayList();
        }
    }
    else
    {
        std::wstring current_file_path = CPlayer::GetInstance().GetCurrentFilePath();
        std::wstring favourite_playlist_path = CPlayer::GetInstance().GetRecentPlaylist().m_favourite_playlist.path;
        CPlaylist playlist;
        playlist.LoadFromFile(favourite_playlist_path);
        if (!CPlayer::GetInstance().IsFavourite())
        {
            //添加到“我喜欢”播放列表
            if (!playlist.IsFileInPlaylist(current_file_path))
            {
                playlist.AddFiles(std::vector<std::wstring> {current_file_path});
                playlist.SaveToFile(favourite_playlist_path);
            }
            CPlayer::GetInstance().SetFavourite(true);
        }
        else
        {
            //从“我喜欢”播放列表移除
            playlist.RemoveFile(current_file_path);
            playlist.SaveToFile(favourite_playlist_path);
            CPlayer::GetInstance().SetFavourite(false);
        }
    }
}


void CMusicPlayerDlg::OnFileOpenUrl()
{
    // TODO: 在此添加命令处理程序代码
    CInputDlg input_dlg;
    input_dlg.SetTitle(CCommon::LoadText(IDS_OPEN_URL));
    input_dlg.SetInfoText(CCommon::LoadText(IDS_INPUT_URL_TIP));
    if (input_dlg.DoModal() == IDOK)
    {
        wstring strUrl = input_dlg.GetEditText().GetString();
        vector<wstring> vecUrl;
        vecUrl.push_back(strUrl);
        CPlayer::GetInstance().OpenFiles(vecUrl);
        UpdatePlayPauseButton();
        DrawInfo(true);
        m_play_error_cnt = 0;
    }
}


void CMusicPlayerDlg::OnPlaylistAddUrl()
{
    // TODO: 在此添加命令处理程序代码
    CInputDlg input_dlg;
    input_dlg.SetTitle(CCommon::LoadText(IDS_ADD_URL));
    input_dlg.SetInfoText(CCommon::LoadText(IDS_INPUT_URL_TIP));
    if (input_dlg.DoModal() == IDOK)
    {
        wstring strUrl = input_dlg.GetEditText().GetString();
        vector<wstring> vecUrl;
        vecUrl.push_back(strUrl);
        CPlayer::GetInstance().AddFiles(vecUrl);
        CPlayer::GetInstance().SaveCurrentPlaylist();
    }
}


afx_msg LRESULT CMusicPlayerDlg::OnSetMenuState(WPARAM wParam, LPARAM lParam)
{
    CMenu* pMenu = (CMenu*)wParam;
    if (pMenu != nullptr)
        SetMenuState(pMenu);
    return 0;
}


void CMusicPlayerDlg::OnLockDesktopLryic()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_lyric_setting_data.desktop_lyric_data.lock_desktop_lyric = !theApp.m_lyric_setting_data.desktop_lyric_data.lock_desktop_lyric;
    m_desktop_lyric.SetLyricWindowLock(theApp.m_lyric_setting_data.desktop_lyric_data.lock_desktop_lyric);
}


void CMusicPlayerDlg::OnCloseDesktopLyric()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_lyric_setting_data.show_desktop_lyric = false;
}


void CMusicPlayerDlg::OnLyricDisplayedDoubleLine()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_lyric_setting_data.desktop_lyric_data.lyric_double_line = !theApp.m_lyric_setting_data.desktop_lyric_data.lyric_double_line;
}


void CMusicPlayerDlg::OnLyricBackgroundPenetrate()
{
    // TODO: 在此添加命令处理程序代码
    theApp.m_lyric_setting_data.desktop_lyric_data.lyric_background_penetrate = !theApp.m_lyric_setting_data.desktop_lyric_data.lyric_background_penetrate;
    m_desktop_lyric.SetLyricBackgroundPenetrate(theApp.m_lyric_setting_data.desktop_lyric_data.lyric_background_penetrate);
}

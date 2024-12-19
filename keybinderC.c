#include <windows.h>
#include <stdio.h>

// Virtual key code mapping
#define VK_COUNT 256
const char* VK_NAMES[VK_COUNT] = {0};

void init_vk_names() {
    // Initialize all to NULL
    for (int i = 0; i < VK_COUNT; i++) {
        VK_NAMES[i] = NULL;
    }
    
    // Basic keys
    VK_NAMES[VK_BACK] = "backspace";
    VK_NAMES[VK_TAB] = "tab";
    VK_NAMES[VK_CLEAR] = "clear";
    VK_NAMES[VK_RETURN] = "enter";
    VK_NAMES[VK_SHIFT] = "shift";
    VK_NAMES[VK_CONTROL] = "ctrl";
    VK_NAMES[VK_MENU] = "alt";
    VK_NAMES[VK_PAUSE] = "pause";
    VK_NAMES[VK_CAPITAL] = "caps_lock";
    VK_NAMES[VK_ESCAPE] = "esc";
    VK_NAMES[VK_SPACE] = "spacebar";
    VK_NAMES[VK_PRIOR] = "page_up";
    VK_NAMES[VK_NEXT] = "page_down";
    VK_NAMES[VK_END] = "end";
    VK_NAMES[VK_HOME] = "home";
    VK_NAMES[VK_LEFT] = "left_arrow";
    VK_NAMES[VK_UP] = "up_arrow";
    VK_NAMES[VK_RIGHT] = "right_arrow";
    VK_NAMES[VK_DOWN] = "down_arrow";
    VK_NAMES[VK_SNAPSHOT] = "print_screen";
    VK_NAMES[VK_INSERT] = "ins";
    VK_NAMES[VK_DELETE] = "del";
    
    // Numbers
    for (int i = 0; i <= 9; i++) {
        char* key = (char*)malloc(2);
        sprintf(key, "%d", i);
        VK_NAMES['0' + i] = key;
    }
    
    // Letters
    for (int i = 0; i < 26; i++) {
        char* key = (char*)malloc(2);
        sprintf(key, "%c", 'a' + i);
        VK_NAMES['A' + i] = key;
    }
    
    // Function keys
    for (int i = 1; i <= 24; i++) {
        char* key = (char*)malloc(4);
        sprintf(key, "F%d", i);
        VK_NAMES[VK_F1 + i - 1] = key;
    }
    
    // Special characters
    VK_NAMES[VK_OEM_PLUS] = "+";
    VK_NAMES[VK_OEM_COMMA] = ",";
    VK_NAMES[VK_OEM_MINUS] = "-";
    VK_NAMES[VK_OEM_PERIOD] = ".";
    VK_NAMES[VK_OEM_2] = "/";
    VK_NAMES[VK_OEM_3] = "`";
    VK_NAMES[VK_OEM_1] = ";";
    VK_NAMES[VK_OEM_4] = "[";
    VK_NAMES[VK_OEM_5] = "\\";
    VK_NAMES[VK_OEM_6] = "]";
    VK_NAMES[VK_OEM_7] = "'";
}

// Get virtual key code from name
BYTE get_vk_code(const char* name) {
    for (int i = 0; i < VK_COUNT; i++) {
        if (VK_NAMES[i] && strcmp(VK_NAMES[i], name) == 0) {
            return (BYTE)i;
        }
    }
    return 0;
}

void click_mouse(int x, int y) {
    SetCursorPos(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
}

void press_key(const char* key) {
    BYTE vk = get_vk_code(key);
    if (vk) {
        keybd_event(vk, 0, 0, 0);
        Sleep(50);
        keybd_event(vk, 0, KEYEVENTF_KEYUP, 0);
    }
}

void press_shift_key(const char* key) {
    BYTE vk = get_vk_code(key);
    if (vk) {
        keybd_event(VK_SHIFT, 0, 0, 0);
        keybd_event(vk, 0, 0, 0);
        Sleep(50);
        keybd_event(vk, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
    }
}

void type_string(const char* str) {
    Sleep(4000); // Initial delay
    
    for (const char* p = str; *p; p++) {
        if (isspace(*p)) {
            press_key("spacebar");
        }
        else if (isupper(*p)) {
            char key[2] = {tolower(*p), 0};
            press_shift_key(key);
        }
        else if (islower(*p)) {
            char key[2] = {*p, 0};
            press_key(key);
        }
        else {
            // Handle special characters
            switch (*p) {
                case '!': press_shift_key("1"); break;
                case '@': press_shift_key("2"); break;
                case '#': press_shift_key("3"); break;
                case '$': press_shift_key("4"); break;
                case '%': press_shift_key("5"); break;
                case '^': press_shift_key("6"); break;
                case '&': press_shift_key("7"); break;
                case '*': press_shift_key("8"); break;
                case '(': press_shift_key("9"); break;
                case ')': press_shift_key("0"); break;
                case '_': press_shift_key("-"); break;
                case '+': press_shift_key("="); break;
                case '{': press_shift_key("["); break;
                case '}': press_shift_key("]"); break;
                case '|': press_shift_key("\\"); break;
                case ':': press_shift_key(";"); break;
                case '"': press_shift_key("'"); break;
                case '<': press_shift_key(","); break;
                case '>': press_shift_key("."); break;
                case '?': press_shift_key("/"); break;
                case '~': press_shift_key("`"); break;
                default:
                    // Try to press as regular key
                    char key[2] = {*p, 0};
                    press_key(key);
            }
        }
        Sleep(50);
    }
}

void hide_pointer() {
    CURSORINFO ci = {sizeof(CURSORINFO)};
    GetCursorInfo(&ci);
    if (ci.flags & CURSOR_SHOWING) {
        ShowCursor(FALSE);
    }
}

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc != 5) {
        printf("Usage: %s <click_x> <click_y> <username> <password>\n", argv[0]);
        return 1;
    }
    
    // Initialize virtual key names
    init_vk_names();
    
    // Parse coordinates
    int click_x = atoi(argv[1]);
    int click_y = atoi(argv[2]);
    const char* username = argv[3];
    const char* password = argv[4];
    
    // Perform automation sequence
    Sleep(3000); // Initial delay
    
    // Click at specified coordinates
    click_mouse(click_x, click_y);
    
    // Type username
    type_string(username);
    
    // Tab to password field
    press_key("tab");
    
    // Type password
    type_string(password);
    
    // Press enter
    press_key("enter");
    
    // Hide pointer
    hide_pointer();
    
    // Cleanup
    for (int i = 0; i < VK_COUNT; i++) {
        if (VK_NAMES[i]) {
            free((void*)VK_NAMES[i]);
        }
    }
    
    return 0;
}
//
// Created by HITO on 01/05/24.
//

#ifndef GIN_MSGBOX_HPP
#define GIN_MSGBOX_HPP

enum class MessageType {
    Info,
    Warning,
    Error,
    Question
};

enum class MessageButton {
    Ok,
    OkCancel,
    YesNo,
    Quit
};

enum class MessageResult {
    None,
    Error,
    Ok,
    Cancel,
    Yes,
    No,
    Close
};

#ifdef GIN_LINUX

#include <gtk/gtk.h>

inline GtkMessageType GetGtkMessageType(MessageType type) {
    switch (type) {
        case MessageType::Info:
            return GTK_MESSAGE_INFO;
        case MessageType::Warning:
            return GTK_MESSAGE_WARNING;
        case MessageType::Error:
            return GTK_MESSAGE_ERROR;
        case MessageType::Question:
            return GTK_MESSAGE_QUESTION;
        default:
            return GTK_MESSAGE_INFO;
    }
}

inline GtkButtonsType GetGtkButtonType(MessageButton button) {
    switch (button) {
        case MessageButton::Ok:
            return GTK_BUTTONS_OK;
        case MessageButton::OkCancel:
            return GTK_BUTTONS_OK_CANCEL;
        case MessageButton::YesNo:
            return GTK_BUTTONS_YES_NO;
        case MessageButton::Quit:
            return GTK_BUTTONS_CLOSE;
        default:
            return GTK_BUTTONS_OK;
    }
}

inline MessageResult GetMessageResult(gint result) {
    switch (result) {
        case GTK_RESPONSE_OK:
            return MessageResult::Ok;
        case GTK_RESPONSE_CANCEL:
            return MessageResult::Cancel;
        case GTK_RESPONSE_YES:
            return MessageResult::Yes;
        case GTK_RESPONSE_NO:
            return MessageResult::No;
        case GTK_RESPONSE_CLOSE:
            return MessageResult::Close;
        default:
            return MessageResult::None;
    }
}

inline MessageResult Show(const char* msg, const char* title, MessageType type, MessageButton button) {
    if (!gtk_init_check(0, nullptr))
        return MessageResult::Error;

    GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GetGtkMessageType(type), GetGtkButtonType(button), msg);
    MessageResult result = GetMessageResult(gtk_dialog_run(GTK_DIALOG(dialog)));
    gtk_widget_destroy(dialog);
    while (g_main_context_iteration(nullptr, false));

    return result;
}

#elif GIN_WIN32

#include <Windows.h>


inline int GetWin32MessageType(MessageType type) {
    switch (type) {
        case MessageType::Info:
            return MB_ICONINFORMATION;
        case MessageType::Warning:
            return MB_ICONWARNING;
        case MessageType::Error:
            return MB_ICONERROR;
        case MessageType::Question:
            return MB_ICONQUESTION;
        default:
            return MB_ICONINFORMATION;
    }
}

inline int GetWin32ButtonType(MessageButton button) {
    switch (button) {
        case MessageButton::Ok:
            return MB_OK;
        case MessageButton::OkCancel:
            return MB_OKCANCEL;
        case MessageButton::YesNo:
            return MB_YESNO;
        case MessageButton::Quit:
            return MB_OK;
        default:
            return MB_OK;
    }
}

inline MessageResult GetMessageResult(int result) {
    switch (result) {
        case 1:
            return MessageResult::Ok;
        case 2:
            return MessageResult::Cancel;
        case 6:
            return MessageResult::Yes;
        case 7:
            return MessageResult::No;
        default:
            return MessageResult::None;
    }
}

inline MessageResult Show(const char* msg, const char* title, MessageType type, MessageButton button) {
    int r = MessageBox(NULL, msg, title, GetWin32MessageType(type) | GetWin32ButtonType(button));
    return GetMessageResult(r);
}

#endif

#endif //GIN_MSGBOX_HPP

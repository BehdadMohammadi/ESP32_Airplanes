#ifndef TELEGRAM_SENDER_H
#define TELEGRAM_SENDER_H

/**
 * @brief Sends a message to a specific Telegram Chat via a Bot.
 * * This function uses the Telegram Bot API to send a text message.
 * It requires the TELEGRAM_TOKEN and TELEGRAM_CHAT_ID to be defined 
 * in your project_config.h.
 * * @param message The text string to be sent to your phone.
 */
void send_telegram_message(const char* message);

#endif
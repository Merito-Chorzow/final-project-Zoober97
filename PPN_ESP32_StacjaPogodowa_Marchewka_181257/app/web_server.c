#include "esp_http_server.h"
#include "esp_log.h"
#include <stdio.h>
#include "ds18b20.h"

#define TAG "WEB"

/* ---------- HTML ---------- */
static const char html_page[] =
"<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset='utf-8'>"
"<title>ESP32 Stacja Pogodowa</title>"
"</head>"
"<body>"
"<h1>Hello World</h1>"
"</body>"
"</html>";


static esp_err_t root_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t temp_get_handler(httpd_req_t *req)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%.2f", ds18b20_read());
    httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void web_server_start()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(httpd_start(&server, &config));

    httpd_uri_t root = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = root_get_handler
    };
    
    httpd_uri_t temp = {
        .uri = "/temp",
        .method = HTTP_GET,
        .handler = temp_get_handler
    };

    httpd_register_uri_handler(server, &root);
    httpd_register_uri_handler(server, &temp);

    ESP_LOGI(TAG, "HTTP server started");
}

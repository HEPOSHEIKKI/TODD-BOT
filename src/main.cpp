#include "config.h"
#include "question-parser.h"
#include "database.h"

dpp::message buildResponseEmbed(std::string questionType, std::string username, std::string question, std::string avatar_url);
dpp::message buildNsfwSettingsEmbed();


    int main()
{   
    std::string tokenFilePath = "token.txt";
    std::string apiToken;
    std::ifstream tokenFile(tokenFilePath);
    initDataBase("database.db");

    if (tokenFile.is_open())
    {

        apiToken.assign((std::istreambuf_iterator<char>(tokenFile)),
                        (std::istreambuf_iterator<char>()));

        tokenFile.close();


    }
    else
    {
        std::cerr << "Error opening file: " << tokenFilePath << std::endl;
        return 1;
    }

    dpp::cluster bot(apiToken);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([](const dpp::slashcommand_t & event)
                        {
	         if (event.command.get_command_name() == "ping") {
                event.thinking(false, dpp::utility::log_error());
                dpp::message msg;
                msg.set_content("Pong!");
	            event.edit_original_response(msg);
	        }
             else if (event.command.get_command_name() == "startgame"){
                event.thinking(false, [=](const dpp::confirmation_callback_t &result){
                    if (!result.is_error()){
                        dpp::message msg;
                        msg.add_embed(
                            dpp::embed()
                                .set_title("Truth or Dare")
                                .set_description("Have fun!"));
                        msg.add_component(
                            dpp::component().add_component(
                                                dpp::component().set_label("Truth").set_type(dpp::cot_button).set_style(dpp::cos_success).set_id("truth-button"))
                                .add_component(
                                    dpp::component().set_label("Dare").set_type(dpp::cot_button).set_style(dpp::cos_danger).set_id("dare-button"))
                                .add_component(
                                    dpp::component().set_label("Random").set_type(dpp::cot_button).set_style(dpp::cos_primary).set_id("random-button")));

                        event.edit_original_response(msg);
                    }
                });
             }
             else if (event.command.get_command_name() == "togglensfw") {
                event.thinking(false, [=](const dpp::confirmation_callback_t &result) {
                    if (!result.is_error()) {
                        event.edit_original_response(buildNsfwSettingsEmbed());
                    }
                });
             } });


    bot.on_ready([&bot](const dpp::ready_t &event)
                 {
	        if (dpp::run_once<struct register_bot_commands>()) {
	            bot.global_command_create(
	                dpp::slashcommand("ping", "Ping pong!", bot.me.id)
	            );
                bot.global_command_create(
                    dpp::slashcommand("startgame", "Starts a game of truth or dare", bot.me.id)
                );
                bot.global_command_create(
                    dpp::slashcommand("togglensfw", "Change NSFW filter settings", bot.me.id)
                );
            } });


    bot.on_button_click([&bot](const dpp::button_click_t &event){
        if (event.custom_id == "truth-button"){
            event.thinking(false, [=](const dpp::confirmation_callback_t &result){
                if (!result.is_error()){
                    dpp::message response = buildResponseEmbed("Truth", event.command.get_issuing_user().username, JsonProcessor::getRandomQuestion("truths", event.command.get_guild().id), event.command.get_issuing_user().get_avatar_url(32, dpp::i_webp, false));
                    event.edit_original_response(response);
                }
            });
            }
        else if (event.custom_id == "dare-button") {
            event.thinking(false, [=](const dpp::confirmation_callback_t &result){
                if (!result.is_error()){
                    dpp::message response = buildResponseEmbed("Dare", event.command.get_issuing_user().username, JsonProcessor::getRandomQuestion("dares", event.command.get_guild().id), event.command.get_issuing_user().get_avatar_url(32, dpp::i_webp, false));
                    event.edit_original_response(response);
                }
            });
        }
        else if (event.custom_id == "random-button") {
            event.thinking(false, [=](const dpp::confirmation_callback_t &result){
                if (!result.is_error()){
                    srand(time(NULL) * rand());
                    int randInt = rand() % 2;
                    if (randInt == 2) {
                        dpp::message response = buildResponseEmbed("Truth", event.command.get_issuing_user().username, JsonProcessor::getRandomQuestion("truths", event.command.get_guild().id), event.command.get_issuing_user().get_avatar_url(32, dpp::i_webp, false));
                        event.edit_original_response(response);
                        }
                    else {
                        dpp::message response = buildResponseEmbed("Dare", event.command.get_issuing_user().username, JsonProcessor::getRandomQuestion("dares", event.command.get_guild().id), event.command.get_issuing_user().get_avatar_url(32, dpp::i_webp, false));
                        event.edit_original_response(response);
                        }
                    }
                    });
            }   
        //TODO: add embeds to the responses
        else if (event.custom_id == "nsfw_disable") {
            event.thinking(false, [=](const dpp::confirmation_callback_t &result){
                if (!result.is_error()){
                    dpp::message msg;
                    msg.set_content("NSFW has been disabled");
                    event.edit_original_response(msg);
                }
            });
            insertData("database.db", event.command.get_guild().id, std::uint64_t(NULL), 1);
        }
        else if (event.custom_id == "nsfw_allow") {
            event.thinking(false, [=](const dpp::confirmation_callback_t &result){
                if (!result.is_error()){
                    dpp::message msg;
                    msg.set_content("NSFW has been allowed");
                    event.edit_original_response(msg);
                }
            });
            insertData("database.db", event.command.get_guild().id, std::uint64_t(NULL), 2);
        }
        else if (event.custom_id == "nsfw_enable") {
            event.thinking(false, [=](const dpp::confirmation_callback_t &result){
                if (!result.is_error()){
                    dpp::message msg;
                    msg.set_content("NSFW has been enabled, normal questions have been disabled");
                    event.edit_original_response(msg);
                }
            });
            insertData("database.db", event.command.get_guild().id, std::uint64_t(NULL), 3);
        }
        });
    bot.start(dpp::st_wait);
}

dpp::message buildResponseEmbed(std::string questionType, std::string username, std::string question, std::string avatar_url) {
    dpp::message msg;
    msg.add_embed(
        dpp::embed()
            .set_title(question)
            .set_footer(
				dpp::embed_footer()
					.set_text(("Type: " + questionType)))
            .set_author("Requested by " + username, "", avatar_url)
            .set_color(16705372));
    msg.add_component(
        dpp::component().add_component(
                            dpp::component().set_label("Truth").set_type(dpp::cot_button).set_style(dpp::cos_success).set_id("truth-button"))
            .add_component(
                dpp::component().set_label("Dare").set_type(dpp::cot_button).set_style(dpp::cos_danger).set_id("dare-button"))
            .add_component(
                dpp::component().set_label("Random").set_type(dpp::cot_button).set_style(dpp::cos_primary).set_id("random-button")));
    return(msg);
}


//TODO:
    //* Decide if this feature should be changeable by normal users or only server mods
    //* CONS: A mod has to step in every time the setting needs changing
    //* PROS: Some servers might want to disable NSFW and the users could change it
    //* Could of course change the allowmode too but I'm too lazy to do it now

dpp::message buildNsfwSettingsEmbed() {
    dpp::message msg;
    msg.add_embed(
        dpp::embed()
            .set_title("NSFW Settings")
            .set_description("Below you can choose from 3 different filtering modes: \n \n **DISABLE:**   Disables NSFW questions.\n **ALLOW:**   Allows NSFW questions.\n **ENABLE:**   Enables NSFW only questions.")
            .set_footer(dpp::embed_footer().set_text("This setting can only be altered by server owners")));
    msg.add_component(
        dpp::component().add_component(
                            dpp::component()
                                .set_label("DISABLE")
                                .set_type(dpp::cot_button)
                                .set_style(dpp::cos_success)
                                  .set_id("nsfw_disable"))
            .add_component(
                dpp::component()
                    .set_label("ALLOW")
                    .set_type(dpp::cot_button)
                    .set_style(dpp::cos_primary)
                    .set_id("nsfw_allow"))
            .add_component(
                dpp::component()
                    .set_label("ENABLE")
                    .set_type(dpp::cot_button)
                    .set_style(dpp::cos_danger)
                    .set_id("nsfw_enable")));
    return msg;
}
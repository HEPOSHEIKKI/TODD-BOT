#include "config.h"
#include "question-parser.h"

dpp::message buildResponseEmbed(std::string questionType, std::string username, std::string question, std::string avatar_url);

    int main()
{


    std::string tokenFilePath = "token.txt";
    std::string apiToken;
    std::ifstream tokenFile(tokenFilePath);

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
	            event.reply("Pong!");
	        }
             else if (event.command.get_command_name() == "startgame"){
                dpp::message msg;
                msg.add_embed(
                    dpp::embed()
                        .set_title("Truth or Dare")
                        .set_description("Why did I sign up for this"));
                msg.add_component(
                    dpp::component().add_component(
                                        dpp::component().set_label("Truth").set_type(dpp::cot_button).set_style(dpp::cos_success).set_id("truth-button"))
                        .add_component(
                            dpp::component().set_label("Dare").set_type(dpp::cot_button).set_style(dpp::cos_danger).set_id("dare-button"))
                        .add_component(
                            dpp::component().set_label("Random").set_type(dpp::cot_button).set_style(dpp::cos_primary).set_id("random-button")));

                event.reply(msg);
             } });


    bot.on_ready([&bot](const dpp::ready_t &event)
                 {
	        if (dpp::run_once<struct register_bot_commands>()) {
	            bot.global_command_create(
	                dpp::slashcommand("ping", "Ping pong!", bot.me.id)
	            );
	        } });
    bot.on_ready([&bot](const dpp::ready_t &event)
                 {
	        if (dpp::run_once<struct register_bot_commands>()) {
	            bot.global_command_create(
	                dpp::slashcommand("startgame", "Starts a game of truth or dare", bot.me.id)
	            );
	        } });

    bot.on_button_click([&bot](const dpp::button_click_t &event)
                        {
        if (event.custom_id == "truth-button"){
            dpp::message response = buildResponseEmbed("Truth", event.command.get_issuing_user().username, JsonProcessor::getRandomQuestion("truths"), event.command.get_issuing_user().get_avatar_url(32, dpp::i_webp, false));
            event.reply(response);
            }
        else if (event.custom_id == "dare-button") {
            dpp::message response = buildResponseEmbed("Dare", event.command.get_issuing_user().username, JsonProcessor::getRandomQuestion("dares"), event.command.get_issuing_user().get_avatar_url(32, dpp::i_webp, false));
            event.reply(response);
        }
        else if (event.custom_id == "random-button") {
            srand(time(NULL) * rand());
            int randInt = rand() % 2;
            if (randInt == 2) {
                dpp::message response = buildResponseEmbed("Truth", event.command.get_issuing_user().username, JsonProcessor::getRandomQuestion("truths"), event.command.get_issuing_user().get_avatar_url(32, dpp::i_webp, false));
                event.reply(response);
            }
            else {
                dpp::message response = buildResponseEmbed("Dare", event.command.get_issuing_user().username, JsonProcessor::getRandomQuestion("dares"), event.command.get_issuing_user().get_avatar_url(32, dpp::i_webp, false));
                event.reply(response);
            }
        }});
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
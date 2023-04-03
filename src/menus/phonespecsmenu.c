// _____________________________________________________________________________
//
//  Phone specs menu
// _____________________________________________________________________________
//
#include "../common.h"

void scrPhoneSpecsMenu(Game *g);
void updatePhoneSpecsMenu(Game *g);
void drawPhoneSpecsMenu(Game *g);

// _____________________________________________________________________________
//
//  Phone specs menu - init function
// _____________________________________________________________________________
//
void scrPhoneSpecsMenu(Game *g) {
    pushMenu(g, 0, NULL, true);
    MENU.updateFunc = updatePhoneSpecsMenu;
    MENU.drawFunc = drawPhoneSpecsMenu;
}

// _____________________________________________________________________________
//
//  Phone specs menu - update function
// _____________________________________________________________________________
//
void updatePhoneSpecsMenu(Game *g) {
    if (K_B_PRESS()) popMenu(g);
}

// _____________________________________________________________________________
//
//  Phone specs menu - draw function
// _____________________________________________________________________________
//
void drawPhoneSpecsMenu(Game *g) {
    drawBoxL(g, 0, 0, 64, 64);

    #define PHONE (MENU.phone)
    #define PHONESPECS (SPECS(PHONE->id))
	
	// draw_sprite(get_phone_sprite(global.phones[selected]), -1, 0, 0)
	// draw_text(70,10, string(global.phones[selected].brand + " " + global.phones[selected].model))
	// draw_text(70,24,"Battery: " + string((global.phones[selected].hp/global.phones[selected].maxHP)*100) + "%")
	// draw_text(160,24,"...Battery Status: " + condition_to_string(global.phones[selected].batteryStatus))
	// draw_text(70,38,"Level " + string(global.phones[selected].level))
	// draw_text(70,52,"EXP: " + string(global.phones[selected].Exp) + "/" + string(global.phones[selected].maxExp))
	// draw_set_valign(fa_top)
	// draw_text(6,70,"AT: " + string(global.phones[selected].attack))
    DrawTexture(shget(g->textures, PHONESPECS.sprite), 0, 0, WHITE);
    drawText(g, TextFormat("%s %s", PHONESPECS.brand, PHONESPECS.model), 70, 10, WHITE);
    drawText(g, TextFormat("Battery: %d%", PHONE->hp/PHONE->maxHP*100), 70, 24);
    drawText(g, TextFormat("...Battery Status: %s", conditionToString[PHONE->batteryStatus]), 160, 24, WHITE);
    drawTextD(g, TextFormat("$ %d", PHONE->level), 70, 38, WHITE);
    drawText(g, TextFormat("EXP: %d/%d", PHONE->exp, PHONE->maxExp), 70, 52, WHITE);
    drawText(g, TextFormat("AT: %d", PHONE->attack), 6, 70, WHITE);

	// switch global.phones[selected].screenStatus
	// {
	// 	case 0:
	// 	draw_set_colour(c_red)
	// 	draw_text(6,70,"------")
	// 	break
	// 	case 1:
	// 	draw_set_colour(c_red)
	// 	draw_text(48,70,"x 0.8")
	// 	break
	// 	case 3:
	// 	draw_set_colour(c_lime)
	// 	draw_text(48,70,"x 1.2")
	// 	break
	// }
	
	// draw_set_colour(c_white)
	// draw_text(6,85,"...Screen Status: " + condition_to_string(global.phones[selected].screenStatus))
	// draw_text(6,105,"DF: " + string(global.phones[selected].defense))
	// switch global.phones[selected].boardStatus
	// {
	// 	case 0:
	// 	draw_set_colour(c_red)
	// 	draw_text(6,105,"------")
	// 	break
	// 	case 1:
	// 	draw_set_colour(c_red)
	// 	draw_text(48,105,"x 0.8")
	// 	break
	// 	case 3:
	// 	draw_set_colour(c_lime)
	// 	draw_text(48,105,"x 1.2")
	// 	break
	// }
	
	// draw_set_colour(c_white)
	// draw_text(6,120,"...Board Status: " + condition_to_string(global.phones[selected].boardStatus))
	// draw_text(6,140,"WT: " + string(global.phones[selected].weight))
	// switch global.phones[selected].coverStatus
	// {
	// 	case 0:
	// 	draw_set_colour(c_red)
	// 	draw_text(6,140,"------")
	// 	break
	// 	case 1:
	// 	draw_set_colour(c_red)
	// 	draw_text(48,140,"x 0.8")
	// 	break
	// 	case 3:
	// 	draw_set_colour(c_lime)
	// 	draw_text(48,140,"x 1.2")
	// 	break
	// }
	
	// draw_set_colour(c_white)
	// draw_text(6,155,"...Cover Status: " + condition_to_string(global.phones[selected].coverStatus))
	
	// draw_text(166,70,"Moves:")
	// draw_text(166,90,global.skills[global.phones[selected].skill1].name)
	// draw_text(166,110,global.skills[global.phones[selected].skill2].name)
	// draw_text(166,130,global.skills[global.phones[selected].skill3].name)
	// draw_text(166,150,global.skills[global.phones[selected].skill4].name)
}
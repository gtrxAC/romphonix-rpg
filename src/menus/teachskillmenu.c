#include "../common.h"

void scrTeachSkillMenu(Phone *phone, int skillID) {
    if (SSPECS(skillID).learnByPhones[phone->id] == '0') {
        pushTextbox("Sorry, this phone cannot learn this skill.", "");
        return;
    }

    for (int i = 0; i < 4; i++) {
        if (phone->skills[i] == skillID) {
            pushTextbox("This phone already knows this skill!", "");
            return;
        }
    }

    // Check if phone has an empty skill slot and can learn instantly
    for (int i = 0; i < 4; i++) {
        if (phone->skills[i] == 0) {
            phone->skills[i] = skillID;
            pushTextbox(F(
                "%s %s learned %s!",
                SPECS(phone->id).brand, SPECS(phone->id).model, SSPECS(skillID).name
            ), "");
            return;
        }
    }

    // This menu is just for keeping variables, the user won't see it
    pushMenu(CB_NOTHING);
    MENU.teachPhone = phone;
    MENU.teachSkillID = skillID;

    // This is the menu that the user will see
    pushTextboxMenu();
    setNextFunc(checkTeachSkillMenu);

    strcpy(MENU.textbox[0], "This phone already knows 4 skills.");
    strcpy(
        MENU.textbox[1],
        F("Which skill do you want to replace with %s?", SSPECS(skillID).name)
    );

    for (int i = 0; i < 4; i++) addChoice(SSPECS(phone->skills[i]).name);
    addChoice("Cancel");
}

void checkTeachSkillMenu() {
    if (MENU.choice == -1 || MENU.choice == 4) {
        popMenu();
        popMenu();
        return;
    }

    // Retrieve the data from the variable keeping menu
    popMenu();
    Phone *phone = MENU.teachPhone;
    int skillID = MENU.teachSkillID;
    popMenu();
    
    phone->skills[MENU.choice] = skillID;
    pushTextbox(F(
        "%s %s learned %s!",
        SPECS(phone->id).brand, SPECS(phone->id).model, SSPECS(skillID).name
    ), "");
    return;
}
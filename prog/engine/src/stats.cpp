#include "stats.h"

Stats::Stats()
{
    strength.name = "Strength"; // fighting, lifting etc, determines speed                          // Fighter, Ranger, Paladin, Brawler, Wrestler
    strength.description = "Strength determines the effectiveness of simple physically demanding actions like fighting, lifting and running. A small temporary loss of strength follows from using it. Strength is replenished during sleep. Use strength and eat to increase strength after sleep. Totally exhausting strength or not using it will result in strength loss";
    strength.dcurrent_dt = 0; // strength only regenerates when sleeping
    strength.dcurrent_duse = -0.01; // sample value, effort = 1.0 around
    strength.dprojected_dt = -3.171e-7; // small decrease in strength by 0.027 per day. So 365 days with inactivity makes you lose 10. 37 days to loose 1 point
    strength.dprojected_duse = 0.5*0.01; // should be some function of day - current....

    agility.name = "Agility"; // dodging, jumping, not tripping, hitting accurately, helps speed    // Ranger, Thief, Scout, Fencer, Acrobat
    agility.description = "Agility determines the effectiveness of actions demanding quickness and precision like dodging, jumping, not tripping, hitting accurately and so on. A temporary loss of agility follows from using it. Agility is replenished fast as balance is regained. Use agility to increase agility after sleep. Not using agility will result in agility loss";
    agility.dcurrent_dt = 2.0; // agility regenerates fast 5 sec to replenish to 10
    agility.dcurrent_duse = -3.0; // sample value one dodge/jump
    agility.dprojected_dt = -3.171e-7; // small decrease in agility by 0.027 per day. So 365 days with inactivity makes you lose 10. 37 days to loose 1 point
    agility.dprojected_duse = 0.5*0.01; // dodge two hundred times a day to increase it by 1.0

    endurance.name = "Endurance"; // running, fighting, not giving in to pain and bloodloss         // Brawler, Fighter, Runner, Ranger, Paladin
    endurance.description = "Endurance determines how long you can run and fight.  A temporary loss of endurance follows from using it. Endurance is replenished in a few minutes after exhaustion. Use endurance to increase endurance after sleep. Not using endurance will result in endurance loss";
    endurance.dcurrent_dt = 0.0333; // endurance slowly regenerates all the time. 5 minutes rest to regain under normal conditions
    endurance.dcurrent_duse = -0.01; // sample value loss per hit effort 1.0 = 0.1. running max effort is possible for 10 seconds, so per second of running should lose 1. Means that run effort per sec = 100
    endurance.dprojected_dt = -3.171e-7; // small decrease in endurance by 0.027 per day. So 365 days with inactivity makes you loose it all. 37 days to loose 1 point
    endurance.dprojected_duse = 0.5*0.01; // half of what you use today is gained extra tomorrow

    instinct.name = "Instinct"; // used as one button to automatically dodge/land good hits, berserk, panic or grab ledges depending on the situation // Barbarian, Ranger, Druid, Adventurer
    instinct.description = "Instinct determines reflexes and intuition. The result of using instinct varies with the context, either it is to dodge, go berserk, panic or grab a ledge. Unless unusually high, instinct can only be used in brief flases of time. Instinct takes a long time to recover. If instinct drops below 0, a persons will to survive is gone. This may result in involuntary suicide attempts if combined with negative confidence. Instinct is inherited, but can be increased by traumatizing events. Instinct will never decrease";
    instinct.dcurrent_dt = 0.0166; // instinct slowly regenerates all the time. 10 minutes rest to regain under normal conditions
    instinct.dcurrent_duse = -3.0; // sample value loss per second of auto control. Should increase regen rate for high values
    instinct.dprojected_dt = 0; // small decrease in instinct by 0.027 per day. So 365 days with inactivity makes you loose it all. 37 days to loose 1 point
    instinct.dprojected_duse = 0; // half of what you use today is gained extra tomorrow

    reason.name = "Reason"; // Used holding and combining magic words mentally into real spells     // Wizard, Mage, Rogue, Druid, Priest
    reason.description = "Reason determines congnitive abilities and mental capacity. It is used for desciphering ancient languages of magic and internalizing mental skills and knowledge. Reason is recovered after a few minutes after mentally tiring activity. If reason ever drops below 0 it is replaced by Madness. This usually results from wizards trying to cast spells beyond their capacity. Reason is increased overnight if used enough. Reason will never decrease permanently unless replaced by madness";
    reason.dcurrent_dt = 0.0333; // reason slowly regenerates all the time. 5 minutes rest to regain under normal conditions
    reason.dcurrent_duse = 1.0; // sample value loss per word of magic currently held.
    reason.dprojected_dt = 0; // the only way to lose reason is to go to far into madness
    reason.dprojected_duse = 2*0.01; // twice of what you use today is gained extra tomorrow

    confidence.name = "Confidence"; // used for social interaction, seduction, avoid panic // Priest, Paladin, Rogue, Fighter
    confidence.description = "Confidence determines inherent social status and self-image. It plays an important role in determining other peoples response to your actions. Confidence is gained from positive reactions and things like friendly battlecries and is lost from negative reactions and enemy battlecries. If confidence ever drops below 0 combined with low instinct, suicide attempts will be made. Confidence is increased overnight if you go to bed with higher confidence than in the morning";
    confidence.dcurrent_dt = 2.7e-4; // here it regenerates one point per hour. Confidence should go to day_value unless in the night,
    confidence.dcurrent_duse = 0.1; // gained from a positive reaction, lost from a negative...
    confidence.dprojected_dt = 0; // projected confidence not a function of time
    confidence.dprojected_duse = 0.5*0.1; // half of what you gain today is gained extra tomorrow

    weight = 65; // kg
    fatigue = 0; // not tired
    health = 15.0;
}

Stats::~Stats()
{
    //dtor
}

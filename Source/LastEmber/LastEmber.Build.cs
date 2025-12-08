using UnrealBuildTool;

public class LastEmber : ModuleRules
{
	public LastEmber(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// --- PUBLICZNE ZALEŻNOŚCI (Główne moduły) ---
		PublicDependencyModuleNames.AddRange(new string[] {
			// Podstawowe silnikowe
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",       // Nowy system inputu (wymagany przez Mover i Lyra)
			"NetCore",             // Podstawy sieciowe

			// --- GAMEPLAY ABILITY SYSTEM (GAS) ---
			"GameplayAbilities",   // Główny moduł systemu umiejętności
			"GameplayTags",        // Tagi (np. State.Dead, Weapon.Rifle)
			"GameplayTasks",       // Zadania asynchroniczne (np. "Czekaj aż animacja się skończy")

			// --- MOVER 2.0 & FIZYKA SIECIOWA ---
			"Mover",               // Nowy komponent ruchu (zastępuje CMC)
			"NetworkPrediction",   // Silnik przewidywania ruchu dla Movera
			// "NetworkPredictionModel", // Modele symulacji

			// --- ZAAWANSOWANE ANIMACJE (Game Animation Sample) ---
			// "MotionMatching",      // System wybierania klatek animacji w locie
			"PoseSearch",          // Baza danych póz (szukanie pasującej animacji)
			// "MotionTrajectory",    // Przewidywanie gdzie postać będzie za chwilę (do pochylania się)
			"AnimGraphRuntime",    // Potrzebne do tworzenia własnych węzłów w AnimGraph

			// --- UI (Interfejs) ---
			"UMG",                 // Podstawowy system widgetów (Unreal Motion Graphics)
			"CommonUI",            // Nowy system UI (obsługa Gamepada, stosy menu)
			"CommonInput"          // Obsługa przełączania ikon (Klawiatura <-> Pad)
		});

		// --- PRYWATNE ZALEŻNOŚCI (Wewnętrzne mechanizmy) ---
		PrivateDependencyModuleNames.AddRange(new string[] {
			"Slate",               // Rdzeń renderowania UI (pod spodem UMG)
			"SlateCore", "GameplayAbilities" // Podstawowe definicje UI
		});

		// Opcjonalnie: Jeśli używasz pluginów Online Subsystem (Steam, EOS)
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
	}
}
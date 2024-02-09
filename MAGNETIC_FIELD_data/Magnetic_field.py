import matplotlib.pyplot as plt
import csv
data={}
######Create Magnteic Field file
# with open("Br_field.csv", "r") as Br_file:
#     colonne1 = []
#     colonne2 = []
#     colonne3 = []
#     colonne4 = []
#     colonne5 = []
#     # Créez des listes vides pour chaque colonne
#     for ligne in Br_file:
#         # Divisez la ligne en valeurs en utilisant l'espace comme séparateur
#         valeurs = ligne.strip().split(",")
#         colonne1.append(float(valeurs[16]))
#         colonne2.append(float(valeurs[17]))
#         colonne3.append(float(valeurs[18]))
#         colonne4.append(float(valeurs[19]))
#         colonne5.append(float(valeurs[20]))
    
# data["Br"] = [colonne1, colonne2, colonne3, colonne4, colonne5]

# with open("Bz_field.csv", "r") as Bz_file:
#     colonne1 = []
#     colonne2 = []
#     colonne3 = []
#     colonne4 = []
#     colonne5 = []
#     # Créez des listes vides pour chaque colonne
#     for ligne in Bz_file:
#         # Divisez la ligne en valeurs en utilisant l'espace comme séparateur
#         valeurs = ligne.strip().split(",")
#         colonne1.append(float(valeurs[16]))
#         colonne2.append(float(valeurs[17]))
#         colonne3.append(float(valeurs[18]))
#         colonne4.append(float(valeurs[19]))
#         colonne5.append(float(valeurs[20]))
    
# data["Bz"] = [colonne1, colonne2, colonne3, colonne4, colonne5]

# with open("Br_field_Z_mm.csv", "r") as z_file:
#     colonne1 = []
#     # Créez des listes vides pour chaque colonne
#     for ligne in z_file:
#         # Divisez la ligne en valeurs en utilisant l'espace comme séparateur
#         valeurs = ligne.strip().split(",")
#         colonne1.append(float(valeurs[0]))
    
# data["z"] = colonne1
# with open("woods_field_complete.txt", "w") as outfile:
#     writer = csv.writer(outfile, delimiter='\t')
#     writer.writerow(["z", "10 mm Bz", "10 mm Br", "28 mm Bz", "28 mm Br", "48 mm Bz", "48 mm Br", "68 mm Bz", "68 mm Br", "88 mm Bz", "88 mm Br"])
#     for i in range(len(data["z"])):
#         writer.writerow([data["z"][i], data["Bz"][0][i], data["Br"][0][i], data["Bz"][1][i], data["Br"][1][i], data["Bz"][2][i], data["Br"][2][i], data["Bz"][3][i], data["Br"][3][i], data["Bz"][4][i], data["Br"][4][i]])
        
##z######10mm:BZ, BR######28mm:BZ, BR###########48mm:BZ, BR###########68mm:BZ, BR###########88mm:BZ, BR


###### Display Magnetic Field
with open("woods_field.txt", "r") as file:
    # Créez des listes vides pour chaque colonne
    colonne1 = []
    colonne2 = []
    colonne3 = []
    colonne4 = []

    # Parcourez chaque ligne du fichier
    for ligne in file:
        # Divisez la ligne en valeurs en utilisant l'espace comme séparateur
        valeurs = ligne.strip().split()

        # Assurez-vous qu'il y a au moins 4 valeurs dans la ligne
        if len(valeurs) >= 4 and "h:x" not in valeurs:
            # Ajoutez chaque valeur à la liste appropriée
            colonne1.append(float(valeurs[0])*100)
            colonne2.append(float(valeurs[1])/6*4)
            colonne3.append(float(valeurs[2]))
            colonne4.append(float(valeurs[3]))

            if float(valeurs[1]) == 3.:
                print(float(valeurs[0]))

plt.plot(colonne1, colonne2, color='grey', zorder=0)

with open("woods_field_complete.txt", "r") as file:
    # Créez des listes vides pour chaque colonne
    colonne1 = []
    colonne2 = []
    colonne3 = []
    colonne4 = []

    # Parcourez chaque ligne du fichier
    for ligne in file:
        # Divisez la ligne en valeurs en utilisant l'espace comme séparateur
        valeurs = ligne.strip().split()

        # Assurez-vous qu'il y a au moins 4 valeurs dans la ligne
        if len(valeurs) >= 4 and "z" not in valeurs:
            # Ajoutez chaque valeur à la liste appropriée
            colonne1.append(-float(valeurs[0])/10)
            colonne2.append(float(valeurs[1]))
            colonne3.append(float(valeurs[2]))
            colonne4.append(float(valeurs[3]))

            if float(valeurs[1]) == 3.:
                print(float(valeurs[0]))
colonne1.reverse()
colonne2.reverse()
plt.plot(colonne1, colonne2, color='grey', zorder=0, ls='--')



plt.xlabel("Front Plastic Position (cm)")
plt.ylabel("B (T)")
plt.scatter(5, 4, color = "black", label = "Actual Position")
plt.scatter(5+25.95, 3, color = "red", label = r"3T - $\Delta = 25.95 cm$")
plt.scatter(5+30.65, 2, color = "orange", label = r"2T - $\Delta = 30.65 cm$")
plt.scatter(5+35.7, 1, color = "green", label = r"1T - $\Delta = 35.7 cm$")

plt.legend(loc="upper left")
plt.show()

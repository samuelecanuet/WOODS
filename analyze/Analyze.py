from ROOT import *
import numpy as np
import re
import os
import subprocess
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib.ticker import MaxNLocator
import random
from root2mpl import *
custom_params = {
        "xtick.direction" : "out",
        "ytick.direction" : "out",
        "lines.markeredgecolor" : "k",
        "lines.markeredgewidth" : 0.5,
        "lines.linewidth" : 1,
        "lines.markersize" : 5,
        "figure.figsize" : (16,9),
        "font.family" : "serif",
        "ytick.labelsize" : 20,
        "xtick.labelsize" : 20,
        "axes.labelsize" : 20,
        "axes.titlesize" : 20,
        "legend.fontsize" : 15,
        "text.usetex" : True,
        # 'figure.subplot.left': 0.20, 
        # 'figure.subplot.bottom': 0.15, 
        # 'figure.subplot.right': 0.95, 
        # 'figure.subplot.top': 0.90
        }
sns.set_theme(style="ticks", rc=custom_params)
    
if __name__ == "__main__":

    file_mono1 = TFile(f"../../../../../../../mnt/hgfs/shared-2/32Ar_47nm_Mylar_mono_1mm.root")
    file_mono2 = TFile(f"../../../../../../../mnt/hgfs/shared-2/32Ar_47nm_Mylar_mono_2mm.root")
    file_monom1 = TFile(f"../../../../../../../mnt/hgfs/shared-2/32Ar_47nm_Mylar_mono_-1mm.root")
    file_monom2 = TFile(f"../../../../../../../mnt/hgfs/shared-2/32Ar_47nm_Mylar_mono_-2mm.root")
    file_mono38 = TFile(f"../../../../../../../mnt/hgfs/shared-2/32Ar_47nm_Mylar_mono_38deg.root")
    file_mono45 = TFile(f"../../../../../../../mnt/hgfs/shared-2/32Ar_47nm_Mylar_mono_45deg.root")
    file_mono48 = TFile(f"../../../../../../../mnt/hgfs/shared-2/32Ar_47nm_Mylar_mono_48deg.root")
    file_mono50 = TFile(f"../../../../../../../mnt/hgfs/shared-2/32Ar_47nm_Mylar_mono_50deg.root")
    file_mono52 = TFile(f"../../../../../../../mnt/hgfs/shared-2/32Ar_47nm_Mylar_mono_52deg.root")
    file_mono = TFile(f"../../../../../../../mnt/hgfs/shared-2/32Ar_47nm_Mylar_mono.root")
    file_dist = TFile(f"../../../../../../../mnt/hgfs/shared-2/32Ar_47nm_Mylar_dist.root")

    # tree = file_mono.Get("Tree")
    # vector_int_variable = vector('int')()
    # dl_vec = vector('double')()
    # e_vec = vector('double')()
    # catcher_vec = vector('double')()
    # tree.SetBranchAddress("Silicon_Detector_Code", vector_int_variable)
    # tree.SetBranchAddress("Silicon_Detector_DL_Deposit_Energy", dl_vec)
    # tree.SetBranchAddress("Silicon_Detector_Deposit_Energy", e_vec)
    # tree.SetBranchAddress("Catcher_Central_Deposit_Energy", catcher_vec)

    # dic_tree = {}
    # dic_tree["Up"] = {}
    # dic_tree["Up"][1] = [TH1D("Catcher_Up1", "Catcher_Up1", 100000,  1e-9, 10000), TH1D("dl_Up1", "dl_Up1", 100000,  1e-9, 10000)]
    # dic_tree["Up"][2] = [TH1D("Catcher_Up2", "Catcher_Up2", 100000,  1e-9, 10000), TH1D("dl_Up2", "dl_Up2", 100000,  1e-9, 10000)]
    # dic_tree["Up"][3] = [TH1D("Catcher_Up3", "Catcher_Up3", 100000,  1e-9, 10000), TH1D("dl_Up3", "dl_Up3", 100000,  1e-9, 10000)]
    # dic_tree["Up"][4] = [TH1D("Catcher_Up4", "Catcher_Up4", 100000,  1e-9, 10000), TH1D("dl_Up4", "dl_Up4", 100000,  1e-9, 10000)]
    # dic_tree["Up"][5] = [TH1D("Catcher_Up5", "Catcher_Up5", 100000,  1e-9, 10000), TH1D("dl_Up5", "dl_Up5", 100000,  1e-9, 10000)]

    # dic_tree["Down"] = {}
    # dic_tree["Down"][1] = [TH1D("Catcher_Down1", "Catcher_Down1", 100000,  1e-9, 100000), TH1D("dl_Down1", "dl_Down1", 100000,  1e-9, 10000)]
    # dic_tree["Down"][2] = [TH1D("Catcher_Down2", "Catcher_Down2", 100000,  1e-9, 100000), TH1D("dl_Down2", "dl_Down2", 100000,  1e-9, 10000)]
    # dic_tree["Down"][3] = [TH1D("Catcher_Down3", "Catcher_Down3", 100000,  1e-9, 100000), TH1D("dl_Down3", "dl_Down3", 100000,  1e-9, 10000)]
    # dic_tree["Down"][4] = [TH1D("Catcher_Down4", "Catcher_Down4", 100000,  1e-9, 100000), TH1D("dl_Down4", "dl_Down4", 100000,  1e-9, 10000)]
    # dic_tree["Down"][5] = [TH1D("Catcher_Down5", "Catcher_Down5", 100000,  1e-9, 100000), TH1D("dl_Down5", "dl_Down5", 100000,  1e-9, 10000)]

    # dic_F = {}
    # dic_F["Up"] = {1: [[98.2, 0.2], [1.94, 0.02]], 2: [[109.7, 0.2], [1.90, 0.02]], 3: [[122.4, 0.2], [1.91, 0.02]], 4: [[137.3, 0.2], [1.95, 0.02]], 5: [[154.7, 0.2], [2.01, 0.02]]}
    # dic_F["Down"] = {1: [[0.76, 0.01], [1.91, 0.02]], 2: [[0.85, 0.01], [1.85, 0.02]], 3: [[0.98, 0.01], [1.88, 0.02]], 4: [[1.09, 0.01], [1.92, 0.02]], 5: [[1.14, 0.01], [2.08, 0.02]]}

    # h_dl = TH1D("h", "h", 500, 0, 50)
    # h_catcher = TH1D("hh", "hh", 100000, 0, 10000)
    # for i in range(tree.GetEntries()):
    #     tree.GetEntry(i)
    #     for j, e_dl in zip(vector_int_variable, dl_vec):
    #         if j < 0:
    #             dir = "Down"
    #         else:
    #             dir = "Up"
    #         if tree.Particle_PDG == 2212:
    #             dic_tree[dir][int(list(str(j))[-1])][0].Fill(tree.Catcher_Central_Deposit_Energy)
    #             dic_tree[dir][int(list(str(j))[-1])][1].Fill(e_dl)
    #         # if j in [-12, -22, -32, -42] and tree.Particle_PDG == 2212:
    #         #     h_dl.Fill(e_dl)
    #         #     h_catcher.Fill(tree.Catcher_Central_Deposit_Energy)

    # for dir in dic_tree.keys():
    #     for strip in dic_tree[dir].values():
    #         strip[0] = [strip[0].GetMean(), strip[0].GetMeanError()]
    #         strip[1] = [strip[1].GetMean(), strip[1].GetMeanError()]

    # figdl, axdl = plt.subplots(1,2)
    # for dir, ax in zip(dic_tree.keys(), axdl):
    #     ax.errorbar([int(key) for key in dic_tree[dir].keys()], [value[1][0] for value in dic_tree[dir].values()], yerr = [value[1][1] for value in dic_tree[dir].values()], capsize=5, marker=".")
    #     ax.errorbar([int(key) for key in dic_F[dir].keys()], [value[1][0] for value in dic_F[dir].values()], yerr = [value[1][1] for value in dic_F[dir].values()], capsize=5, marker=".", label="Federica")
    #     ax.set_title(dir)
    #     ax.set_xlabel("Strip nb")
    #     ax.set_ylabel("Mean")
    # plt.legend()
    # figdl.suptitle("Dead Layer")

    # figcatcher, axcatcher = plt.subplots(1,2)
    # for dir, ax in zip(dic_tree.keys(), axcatcher):
    #     ax.errorbar([int(key) for key in dic_tree[dir].keys()], [value[0][0] for value in dic_tree[dir].values()], yerr = [value[0][1] for value in dic_tree[dir].values()], capsize=5, marker=".")
    #     ax.errorbar([int(key) for key in dic_F[dir].keys()], [value[0][0] for value in dic_F[dir].values()], yerr = [value[0][1] for value in dic_F[dir].values()], capsize=5, marker=".", label="Federica")
    #     ax.set_title(dir)
    #     ax.set_xlabel("Strip nb")
    #     ax.set_ylabel("Mean")
    # plt.legend()
    # figcatcher.suptitle("Catcher")

    # fige, axe =  plt.subplots(1,2)
    # for dir, ax in zip(dic_tree.keys(), axe):
    #     ax.errorbar([int(key) for key in dic_tree[dir].keys()], [3356-value[0][0]-value[1][0] for value in dic_tree[dir].values()], yerr = [value[0][1]+value[1][1]for value in dic_tree[dir].values()], capsize=5, marker=".")
    #     ax.errorbar([int(key) for key in dic_F[dir].keys()], [3356-value[0][0]-value[1][0] for value in dic_F[dir].values()], yerr = [value[0][1]+value[1][1]for value in dic_F[dir].values()], capsize=5, marker=".", label="Federica")
    #     ax.set_title(dir)
    #     ax.set_xlabel("Strip nb")
    #     ax.set_ylabel("Mean")
    # fige.suptitle("e")
    # plt.legend()
    # plt.show()

    file_list = [file_mono, 
                 file_mono38, 
                 file_mono45, 
                 file_mono38,
                 file_mono50, 
                 file_mono52
                 ] 
    
    file_list = [file_mono, 
                 file_mono1, 
                 file_monom1, 
                 file_mono2
                 ] 

    file_list = [file_mono, 
                 file_dist]


    dic_F = {}
    dic_F["Up"] = {1: [3255.86, 0.03], 2: [3244.4, 0.03], 3: [3231.69, 0.03], 4: [3216.75, 0.03], 5: [3199.29, 0.03]}
    dic_F["Down"] = {1: [3353.33, 0.03], 2: [3353.3, 0.03], 3: [3353.14, 0.03], 4: [3352.99, 0.03], 5: [3352.78, 0.03]}
    
    dic = {}
    dic_histo = {}
    for file in file_list:
        dic[file.GetName()] = {}
        dic_histo[file.GetName()] = {}
        for dir in ["Up", "Down"]: 
            dic[file.GetName()][dir]={}
            dic_histo[file.GetName()][dir]={}
            
            for strip in range(1, 6):   
                histo = file.Get(f"1{dir}_Strip_{strip}_coinc").Clone("h_sum")
                histo_no = file.Get(f"1{dir}_Strip_{strip}_nocoinc").Clone("h_sum_no")
                for det in range(2, 5):
                    h=file.Get(f"{det}{dir}_Strip_{strip}_coinc")
                    h_no = file.Get(f"{det}{dir}_Strip_{strip}_nocoinc")
                    histo.Add(h, 1.0)
                    histo_no.Add(h_no, 1.0)

                histo.Add(histo_no, 1.0)

                histo.GetXaxis().SetRangeUser(3100, 3400)
                dic[file.GetName()][dir][strip] = [histo.GetMean(), histo.GetMeanError()]
                dic_histo[file.GetName()][dir][strip]=histo
                # print(histo.GetEntries())
                # print(histo.GetMeanError(), histo.GetRMS()/np.sqrt(histo.GetEntries()-1))
                # fig, ax = plt.subplots()
                # DisplayTH1D(histo, ax, xlim=(3100, 3400), title="", label="")
                # plt.show()


    ####DISPLAY histograms####
    fig_hist, ax_hist = plt.subplots(1, 2, gridspec_kw= {"wspace" : 0.3})
    for i, dir in enumerate(["Up", "Down"]): 
        ax1 = ax_hist[i]
        counter = 0
        for file, color in zip(file_list, ["red", "blue", "orange", "green", "brown", "pink", "purple"]):
            counter+=1
            if "mono" in file.GetName():
                label="mono-energetic"
                offset = -0.1
            else:
                label="distribution"
                offset = 0.1    
            
            if dir == "Down":
                xlimd = 3300
                y = 0.08+0.01*counter
            else:
                xlimd = 3100
                y = 0.008+0.001*counter
            
            histo = dic_histo[file.GetName()][dir][1]
            histo.GetXaxis().SetRangeUser(xlimd, 3400)
            DisplayTH1D(histo, ax1, title="", xlim=(xlimd, 3400), label = label + f"\t: {round(histo.GetMean(), 2)} keV", rebin=4, normalized = True, color=color)
            ax1.set_title(dir)
            ax1.set_ylabel("Renormalized(/0.4keV)")
            ax1.set_xlabel("Energy(keV)")
            ax1.grid(True)
            ax1.legend(loc="upper right")
    plt.show()

    fig, ax = plt.subplots(1, 2, gridspec_kw= {"wspace" : 0.3})
    for i, dir in enumerate(["Up", "Down"]): 
        ax1 = ax[i]
        ax1.set_title(dir)
        ax1.set_xlabel("Strip nb")
        ax1.set_ylabel("Mean Energy(keV)")
        ax1.grid(True)
        for file, color in zip(file_list, ["red", "blue", "orange", "green", "brown", "pink", "purple"]):
            if "mono" in file.GetName():
                label="mono-energetic"
                offset = -0.1
            else:
                label="distribution"
                offset = 0.1            
            if "_1mm" in file.GetName():
                label += " 1mm"
            if "_-1mm" in file.GetName():
                label += " -1mm"
            if "_2mm" in file.GetName():
                label += " 2mm"
            if "_-2mm" in file.GetName():
                label += " -2mm"

            if "38deg" in file.GetName():
                label += "38deg"

            if "52deg" in file.GetName():
                label += "52deg"

            if "45deg" in file.GetName():
                label += "45deg"

            if "48deg" in file.GetName():
                label += "48deg"

            if "50deg" in file.GetName():
                label += "50deg"
            ax1.errorbar([int(key)+offset for key in dic[file.GetName()][dir].keys()], [value[0] for value in dic[file.GetName()][dir].values()], yerr = [value[1] for value in dic[file.GetName()][dir].values()], color=color, capsize=5, marker=".", label=label)
        #ax1.errorbar([int(key) for key in dic_tree[dir].keys()], [3356-value[0][0]-value[1][0] for value in dic_tree[dir].values()], yerr = [value[0][1]+value[1][1]for value in dic_tree[dir].values()], capsize=5, marker="." ,label="sous")
        ax1.errorbar([int(key) for key in dic_F[dir].keys()], [value[0] for value in dic_F[dir].values()], yerr = [value[1] for value in dic_F[dir].values()], color='black', capsize=5, marker=".", label="mono-energetic Federica", ms=10, fmt='s')
    plt.legend()
    plt.show()

    
    # resultat = subprocess.run("g++ -o tree_reader tree_reader.cpp `root-config --cflags --libs`", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    ##/////////////////////////////////////////////////////for ./back diff material
    # fig, axs = plt.subplots()
    # dic={}
    # for mat, thres, color in zip(["Pl", "Si"], [100, 100], ["blue", "orange"]):
    #     dic[mat] = [[], [], []]
    #     for i in [0.5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]:
    #         filename = f"32Ar_a1_b0_{i}_{mat}.root"

            
    #         file = TFile(filename)
    #         hist = file.Get("h1d_PL_Edep_positron")
    #         dic[mat][2].append(hist.Integral())
    #         hist.GetXaxis().SetRangeUser(0, thres)
    #         dic[mat][1].append(hist.Integral())
    #         dic[mat][0].append(i)

    #     print(dic[mat][0])
    #     axs.errorbar(dic[mat][0], np.array(dic[mat][1])/np.array(dic[mat][2])*100, label = mat+" Threshoold = "+str(thres)+" keV", markersize = 10, yerr = np.array(np.sqrt(dic[mat][1]))/np.array(dic[mat][2])*100, capsize=5, ecolor="black", color=color)
    #     axs.set_xticks(np.linspace(0, 10, 11))
    #     axs.set_xlabel("Inital Kinetic energy (MeV)")
    #     axs.set_ylabel("\% Losses")
    
    #     axs.set_title(r"$\beta$ Detector Material Study")
    # plt.legend()
    # plt.show()
    ##/////////////////////////////////////////////////////////////// for ./back diff T
    # fig, axs = plt.subplots()
    # dic={}
    # for tesla, thres, color in zip(["1T", "2T", "3T", "Pl"], [100, 100, 100, 100], ["blue", "orange", "red", "green"]):
    #     dic[tesla] = [[], [], []]
    #     for i in [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]:
    #         filename = f"32Ar_a1_b0_{i}_{tesla}.root"
    #         if tesla == "Pl": label = "4T" 
    #         else: label = tesla

    #         file = TFile(filename)
    #         hist = file.Get("h1d_PL_Edep_positron")
    #         hist1 = file.Get("h1d_E0_positron")
    #         dic[tesla][2].append(hist.Integral())
    #         hist.GetXaxis().SetRangeUser(0, thres)
    #         dic[tesla][1].append(hist.Integral())
    #         dic[tesla][0].append(i)
        
    #     axs.errorbar(dic[tesla][0], np.array(dic[tesla][1])/np.array(dic[tesla][2])*100, label = label, markersize = 10, yerr = np.array(np.sqrt(np.array(dic[tesla][1]))/np.array(dic[tesla][2]))*100, fmt=".", capsize=5, ecolor="black", color=color)
    #     axs.set_xticks(np.linspace(0, 10, 11))
    #     axs.set_xlabel("Inital Kinetic energy (MeV)")
    #     axs.set_ylabel("\% Losses")
    #     axs.set_title(r"$\beta$ Detector Magnetic Field Study")
    # plt.legend()
    # plt.show()
    #plt.savefig(f"Pl_position.png", dpi=300)

    ###
    # colors=["blue", "orange", "red", "green", "purple", "brown", "black", "cyan", "grey", "yellow"]
    
    # dic={}
    # for i in [1,2]:
    #     fig, axs = plt.subplots()
    #     for tesla, color in zip(["1T", "2T", "3T", "4T"],  ["blue", "orange", "red", "green"]):
    #         filename = f"32Ar_a1_b0_{i}_{tesla}.root"
    #         if tesla == "Pl": label = "4T" 
    #         else: label = tesla

    #         file = ROOT_DISPLAY(TFile(filename))
    #         print(file.GetHist("h1d_PL_Angle_positron").Integral())
    #         print(file.GetHist("h1d_PL_E0_positron").Integral())
    #         ax = file.Display("h1d_PL_Angle_positron", axs, normalized = True, label = label, color=color, title=f"Plastic Scintillator {i} MeV Positron Hit Angle")
    #     plt.legend()
    #     plt.show()
        #plt.savefig(f"Pl_position_{i}_MeV.png", dpi=300)


    ###larmor
    # B = np.linspace(1, 4, 4)
    # E = np.linspace(0, 10e6, 1e3)
    # me = 511*10**3
    # c = 3.0*10**8
    # def R(E, B):
    #     return me/B/c*np.sqrt(((E+me)/me)**2-1)*10**2
    
    # for b in B:
    #     plt.plot(E*1e-6, R(E, b), label=f"{b} T")
    # plt.xlabel("Positron energy MeV")
    # plt.ylabel("Lamor Radius (cm)")
    #plt.show()

    # analyzer = ROOT_DISPLAY(TFile("../../../../../../../mnt/hgfs/shared-2/32Ar_a1_b0_1.root"))
    # for key, value in analyzer.GetEshiftDictionnary().items():
    #     print(key[:-1] + " Strip "+key[-1] +": Eshift = {:.2f} +/- {:.2f} keV".format(value[0], value[1]))




#ifndef KSHORTRECONSTRUCTION_H
#define KSHORTRECONSTRUCTION_H

#include <fun4all/SubsysReco.h>

#include <trackbase/ActsTrackingGeometry.h>

#include <Eigen/Dense>

class TFile;
class TH1;
class TNtuple;

class ActsGeometry;
class PHCompositeNode;
class SvtxTrack;
class SvtxTrackMap;
class SvtxVertexMap;

class KshortReconstruction : public SubsysReco
{
 public:
  KshortReconstruction(const std::string& name = "KshortReconstruction");
  virtual ~KshortReconstruction() = default;

  int InitRun(PHCompositeNode* topNode) override;
  int process_event(PHCompositeNode* topNode) override;
  int End(PHCompositeNode* /*topNode*/) override;

  void setPtCut(double ptcut) { invariant_pt_cut = ptcut; }
  void setTrackPtCut(double ptcut) { track_pt_cut = ptcut; }
  void setTrackQualityCut(double cut) { _qual_cut = cut; }
  void setPairDCACut(double cut) { pair_dca_cut = cut; }
  void setTrackDCACut(double cut) { track_dca_cut = cut; }
  void setRequireMVTX(bool set) { _require_mvtx = set; }
  void setDecayMass(float decayMassSet) { decaymass = decayMassSet; }  //(muons decaymass = 0.1057) (pions = 0.13957) (electron = 0.000511)
  void set_output_file(const std::string& outputfile) { filepath = outputfile; }
  void save_tracks(bool save = true) { m_save_tracks = save; }

 private:
  void fillNtp(SvtxTrack* track1, SvtxTrack* track2, Acts::Vector3 dcavals1, Acts::Vector3 dcavals2, Acts::Vector3 pca_rel1, Acts::Vector3 pca_rel2, double pair_dca, double invariantMass, double invariantPt, float invariantPhi, float rapidity, float pseudorapidity, Eigen::Vector3d projected_pos1, Eigen::Vector3d projected_pos2, Eigen::Vector3d projected_mom1, Eigen::Vector3d projected_mom2, Acts::Vector3 pca_rel1_proj, Acts::Vector3 pca_rel2_proj, double pair_dca_proj,unsigned int track1_silicon_cluster_size, unsigned int track2_silicon_cluster_size, unsigned int track1_mvtx_cluster_size, unsigned int track1_mvtx_state_size, unsigned int track1_intt_cluster_size, unsigned int track1_intt_state_size, unsigned int track2_mvtx_cluster_size, unsigned int track2_mvtx_state_size, unsigned int track2_intt_cluster_size, unsigned int track2_intt_state_size, int runNumber, int eventNumber);

  void fillHistogram(Eigen::Vector3d mom1, Eigen::Vector3d mom2, TH1* massreco, double& invariantMass, double& invariantPt, float& invariantPhi, float& rapidity, float& pseudorapidity);

  // void findPcaTwoTracks(SvtxTrack *track1, SvtxTrack *track2, Acts::Vector3& pca1, Acts::Vector3& pca2, double& dca);
  void findPcaTwoTracks(const Acts::Vector3& pos1, const Acts::Vector3& pos2, Acts::Vector3 mom1, Acts::Vector3 mom2, Acts::Vector3& pca1, Acts::Vector3& pca2, double& dca) const;

  int getNodes(PHCompositeNode* topNode);

  Acts::Vector3 calculateDca(SvtxTrack* track, const Acts::Vector3& momentum, Acts::Vector3 position);

  bool projectTrackToCylinder(SvtxTrack* track, double Radius, Eigen::Vector3d& pos, Eigen::Vector3d& mom);
  bool projectTrackToPoint(SvtxTrack* track, Eigen::Vector3d PCA, Eigen::Vector3d& pos, Eigen::Vector3d& mom);

  Acts::Vector3 getVertex(SvtxTrack* track);
  static std::vector<unsigned int> getTrackStates(SvtxTrack *track);
  
  TNtuple* ntp_reco_info {nullptr};
  ActsGeometry* _tGeometry {nullptr};
  SvtxTrackMap* m_svtxTrackMap {nullptr};
  SvtxVertexMap* m_vertexMap {nullptr};
  
  std::string filepath {""};
  float decaymass {0.13957};  // pion decay mass
  bool _require_mvtx {true};
  double _qual_cut {1000.0};
  double pair_dca_cut {0.05};  // kshort relative cut 500 microns
  double track_dca_cut {0.01};
  double invariant_pt_cut {0.1};
  double track_pt_cut {0.2};
  TFile* fout {nullptr};
  TH1* recomass {nullptr};

  bool m_save_tracks {false};
  SvtxTrackMap *m_output_trackMap {nullptr};
  std::string m_output_trackMap_node_name {"KshortReconstruction_SvtxTrackMap"};
};

#endif  // KSHORTRECONSTRUCTION_H

//
//  GameRoomVC.swift
//  ChattoApp
//
//  Created by Goodnews on 2018. 7. 21..
//  Copyright © 2018년 Badoo. All rights reserved.
//

import UIKit
import SnapKit
import Then

class GameRoomVC: AddRandomMessagesChatViewController {
	
	var stateView: GameStateView!
	
	override func viewDidLoad() {
		super.viewDidLoad()
		
		setupStateView()
		setupSocket()
	}
	
	private func setupSocket() {
		SocketIOManager.shared.establishConnection {
			SocketIOManager.shared.onStartGame() { data in
				self.didReceiveQuestion(data: data)
			}
			SocketIOManager.shared.onSendChat() { data in
				self.didReceiveQuestion(data: data)
			}
			SocketIOManager.shared.onCorrect() { data in
				if data[0] as! String == "정답입니다." {
					self.stateView.getTheAnswer()
				} else {
					print("오오답")
				}
			}
			SocketIOManager.shared.onAnswer()
	
		}
	}

	private func didReceiveQuestion(data: [Any]) {
		guard let data = data[0] as? NSDictionary else {
			print("---------VOV 상세 데이터를 찾을 수 없습니다.")
			return
		}//				Constants.nickname = dic["key"]!
		self.stateView.questionLabel.text = data.object(forKey: "key") as! String
		
		stateView.startGame()
	}
	private func setupStateView() {
		self.navigationController?.navigationBar.barTintColor = .lavender
		
		collectionView.bounces = false
		
		stateView = GameStateView()
		
		view.addSubview(stateView)
		
		
		collectionView.snp.remakeConstraints { make -> Void in
			make.top.equalTo(stateView.snp.bottom)
			make.left.right.equalTo(self.view)
			make.bottom.equalTo(self.view)
		}

		stateView.snp.remakeConstraints { make -> Void in
			if #available(iOS 11.0, *) {
				make.top.left.right.equalTo(view.safeAreaLayoutGuide)
			} else {
				make.top.left.right.equalTo(self.view)
			}
			make.height.equalTo(86)
		}
		
		
		stateView.startButton.addTarget(self, action: #selector(didTapStart), for: .touchUpInside)
	}
	
	@objc private func didTapStart() {
		SocketIOManager.shared.emitStart()
		
		stateView.snp.updateConstraints { make -> Void in
			make.height.equalTo(132)
		}
		
		
		
		stateView.startGame()
		
		UIView.animate(withDuration: 0.5) {
			self.view.layoutIfNeeded()
		}
		
	}
	
	
	//
	// MARK: ScrollView Methods
	//
	private var lastContentOffset: CGFloat = 0
	
	
	override func scrollViewDidScroll(_ scrollView: UIScrollView) {
		
//		if (self.lastContentOffset < scrollView.contentOffset.y) {
//
//
//			self.stateView.snp.updateConstraints { make -> Void in
//				make.height.equalTo(86)
//			}
//
//		} else if (self.lastContentOffset > scrollView.contentOffset.y) {
//
//			self.stateView.snp.updateConstraints { make -> Void in
//				make.height.equalTo(0)
//			}
//		}
		
		UIView.animate(withDuration: 0.3) {
			self.view.layoutIfNeeded()
		}
		
		self.lastContentOffset = scrollView.contentOffset.y
		
	}
}

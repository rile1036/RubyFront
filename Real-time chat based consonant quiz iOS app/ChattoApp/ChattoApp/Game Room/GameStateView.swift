//
//  GameStateView.swift
//  ChattoApp
//
//  Created by Goodnews on 2018. 7. 21..
//  Copyright © 2018년 Badoo. All rights reserved.
//

import UIKit
import SnapKit
import Then

class GameStateView: UIView {
	
	var startButton = UIButton().then {
		$0.backgroundColor = .sunflower
		$0.layer.cornerRadius = 22
		$0.setTitle("지금 게임 시작하기", for: .normal)
		$0.setTitleColor(.lavender, for: .normal)
		$0.titleLabel?.font = UIFont.systemFont(ofSize: 17, weight: .semibold)
	}
	
	var questionLabel = UILabel().then {
		$0.font = UIFont.systemFont(ofSize: 28, weight: .bold)
		$0.textAlignment = .center
		$0.textColor = .white
		
	}
	
	var leftImgView = UIImageView(image: #imageLiteral(resourceName: "graphicSprinkle2"))
	var rightImgView = UIImageView(image: #imageLiteral(resourceName: "graphicSprinkle2"))
	
	override init(frame: CGRect) {
		super.init(frame: .zero)
		
		setupView()
	}
	
	required init?(coder aDecoder: NSCoder) {
		fatalError("init(coder:) has not been implemented")
	}
	
	func setupView() {
		self.backgroundColor = .lavender
	
		
		addSubview(startButton)
		addSubview(questionLabel)
		
		addSubview(leftImgView)
		addSubview(rightImgView)
		
		questionLabel.isHidden = true
		leftImgView.isHidden = true
		rightImgView.isHidden = true
		
		startButton.snp.remakeConstraints { make -> Void in
			make.bottom.equalTo(self).inset(26)
			make.size.equalTo(CGSize(width: 272, height: 44))
			make.centerX.equalTo(self)
		}
		
		questionLabel.snp.remakeConstraints { make -> Void in
			make.top.equalTo(startButton.snp.bottom).offset(16)
			make.left.right.equalTo(self)
		}

		leftImgView.snp.remakeConstraints { make -> Void in
			make.left.equalTo(16)
			make.top.equalTo(8)
		}
		rightImgView.snp.remakeConstraints { make -> Void in
			make.top.equalTo(8)
			make.right.equalTo(self).inset(16)
		}


		
	}
	
	func startGame() {
		startButton.snp.remakeConstraints { make -> Void in
			make.centerX.equalTo(self)
			make.top.equalTo(20)
			make.size.equalTo(CGSize(width: 76, height: 24))
		}
		
		questionLabel.isHidden = false
		leftImgView.isHidden = true
		rightImgView.isHidden = true
		
		startButton.backgroundColor = .white
		startButton.layer.cornerRadius = 12
		startButton.setTitle("1 ROUND", for: .normal)
		startButton.setTitleColor(.vividPurple, for: .normal)
		startButton.titleLabel?.font = UIFont.systemFont(ofSize: 12, weight: .semibold)
	}
	
	func getTheAnswer() {
		leftImgView.isHidden = false
		rightImgView.isHidden = false
		
		questionLabel.text = "정답입니다!"
	}
}

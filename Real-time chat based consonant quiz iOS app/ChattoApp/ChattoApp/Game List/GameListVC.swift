//
//  GameListVC.swift
//  ChattoApp
//
//  Created by Goodnews on 2018. 7. 20..
//  Copyright © 2018년 Badoo. All rights reserved.
//

import UIKit
import SnapKit
import RxCocoa
import RxSwift

fileprivate let cellID = "gameRoomCellID"
fileprivate let headerID = "gameRoomHeader"
class GameListVC: ViewController {
	
	@IBOutlet weak var nicknameOutlet: UIBarButtonItem!
	@IBOutlet weak var collectionView: UICollectionView!
	
	override func viewDidLoad() {
		super.viewDidLoad()
		
		setupNavBar()
		setupCollectionView()
		//		setupRx()
	}
	
	private func setupNavBar() {
		// image Title
		
		let nicknameButton = UIBarButtonItem(title: Constants.nickname, style: .plain, target: self, action: nil)
		self.navigationItem.rightBarButtonItem = nicknameButton
		
		let image : UIImage = #imageLiteral(resourceName: "navTitle")
		let imageView = UIImageView(frame: CGRect(x: 0, y: 0, width: 68, height: 17))
		imageView.contentMode = .scaleAspectFit
		imageView.image = image
		self.navigationItem.titleView = imageView
		
		self.navigationController?.navigationBar.shadowImage = UIImage()
	}
	
	private func setupCollectionView() {
		collectionView.register(UINib(nibName: "GameRoomCell", bundle: nil), forCellWithReuseIdentifier: cellID)
		self.collectionView.delegate = self
		self.collectionView.dataSource = self
	}
}

extension GameListVC: UICollectionViewDelegate, UICollectionViewDelegateFlowLayout, UICollectionViewDataSource {
	func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, sizeForItemAt indexPath: IndexPath) -> CGSize {
		let width: CGFloat = self.view.frame.width - 44
		let height: CGFloat = 104
		return CGSize(width: width, height: height)
	}
	
	func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
		return 1
	}
	
	func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
		let cell = collectionView.dequeueReusableCell(withReuseIdentifier: cellID, for: indexPath) as! GameRoomCell
		
		
		return cell
	}
	
	func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, minimumLineSpacingForSectionAt section: Int) -> CGFloat {
		return 16
	}
	
	func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, minimumInteritemSpacingForSectionAt section: Int) -> CGFloat {
		return 16
	}
	
	func collectionView(_ collectionView: UICollectionView, didSelectItemAt indexPath: IndexPath) {
		let dataSource = DemoChatDataSource(count: 0, pageSize: 50)
		let viewController = GameRoomVC()
		viewController.dataSource = dataSource
		self.navigationController?.pushViewController(viewController, animated: true)
	}
	//
	// Header & footer
	func collectionView(_ collectionView: UICollectionView, viewForSupplementaryElementOfKind kind: String, at indexPath: IndexPath) -> UICollectionReusableView {
		
		let headerView = collectionView.dequeueReusableSupplementaryView(ofKind: kind, withReuseIdentifier: headerID, for: indexPath) as! GameRoomHeader
		
		headerView.createRoomButton.addTarget(self, action: #selector(didTapCreateRoom), for: .touchUpInside)
		return headerView
		
	}
	
	@objc private func didTapCreateRoom() {
		let popupView = CreateGameRoomView.initFromNib()
		let window = UIApplication.shared.keyWindow
		window?.addSubview(popupView)
		popupView.snp.remakeConstraints { make -> Void in
			make.edges.equalTo(window!)
		}
	}
	
	func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, referenceSizeForHeaderInSection section: Int) -> CGSize {
		let width:CGFloat = self.view.frame.width
		let height:CGFloat = 200
		
		return CGSize(width: width, height: height)
	}
}


